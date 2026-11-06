# LMDB Port Implementation Notes

## Status

The LMDB backend has not yet been implemented. Do not treat earlier illustrative code snippets or workflow examples in conversation as a ready-to-apply or tested patch.

A safe backend migration requires changes to the complete current source/build files and tests, followed by platform builds. In particular, replacing LevelDB cannot be done correctly by changing only a linker flag or mechanically replacing LevelDB types.

## Existing wrapper contract to preserve

The current `CDBWrapper` layer is used by chainstate and block-index code. A replacement must preserve its externally used operations and their semantics:

- serialization of keys and values through the existing `DataStream` / `CDataStream` code;
- value obfuscation behavior;
- atomic batch writes;
- ordered cursor scans and lower-bound `Seek` behavior;
- error handling and empty/not-found behavior;
- `StoragePath()`, memory-only behavior, wipe behavior, and cache/map sizing expectations.

## LMDB-specific requirements

- Implement `Seek(key)` using a lower-bound cursor operation (`MDB_SET_RANGE`), not exact-match lookup.
- Keep read transactions alive for the lifetime of an iterator, and close the cursor before aborting the read transaction.
- A write batch must apply all operations in order in one write transaction. Abort on any error, and commit only after all operations succeed.
- `fSync` must have a documented and implemented durability mapping; LMDB commit/sync flags must not be silently ignored.
- `memory_only` requires a deliberate implementation. It must not silently open a persistent LMDB environment at the supplied path.
- LMDB's map size is not equivalent to LevelDB's cache size. Choose and document a separate map-size policy, handle `MDB_MAP_FULL`, and provide a safe resize strategy if required.
- The LMDB file/directory layout differs from LevelDB. Existing LevelDB data is not readable by LMDB; provide a migration/export plan or explicitly require reindex/rebuild with a backup warning.
- `EstimateSize`, `DynamicMemoryUsage`, and `force_compact` need documented LMDB-compatible behavior rather than misleading LevelDB-equivalent values.

## Build integration requirements

This repository uses autotools for its primary build. LMDB needs to be built or discovered for each target platform, and its include/library flags must reach the final executable link commands. Linking LMDB only into a static archive's `LIBADD` variable is insufficient if that variable is not used by the final link target.

For self-contained release binaries, build a static LMDB library for the target toolchain and verify the final executable's dependencies. Do not use a generic `-static` linker flag across Linux, macOS, and Windows; those platforms have different static-linking constraints and system-library requirements.

## Required validation before merge

1. Inspect and update all active build definitions, including the autotools source lists and LevelDB include fragments.
2. Search the complete repository for LevelDB-specific APIs and any extra LevelDB-backed storage adapters.
3. Add DB wrapper tests for binary keys/values, not-found, overwrite, erase, ordered lower-bound seek, iteration, batch atomicity, obfuscation, wipe, memory-only, and reopen persistence.
4. Test failure paths, including map-full handling and transaction aborts.
5. Build and run tests on Linux, macOS, and Windows using the supported toolchains.
6. Test the upgrade story with a copy of an existing LevelDB data directory; never test destructive migration on a user's only copy.
7. Verify packaged binaries do not require an unintended dynamic LMDB runtime library.

## Caution

Until those implementation and validation steps are complete, do not distribute LMDB-backed binaries or open an existing LevelDB data directory with the LMDB backend. The database formats are incompatible, and a backend change can cause loss of local chainstate/index data if migration and recovery are not handled explicitly.
