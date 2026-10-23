---
summary: ""
problem: ""
root_cause: ""
changes: ""
validation: ""
checklist: []
---

# Patch Template

## Summary

Describe the change briefly and state the goal of the patch.

## Problem

Describe the problem being fixed or improved.

## Root cause

Describe the underlying cause and why the change is necessary.

## Proposed change

- Backend change:
- Files touched:
- Compatibility considerations:
- Behavior preserved:

## Patch outline

```diff
--- a/src/dbwrapper.h
+++ b/src/dbwrapper.h
@@
-#include <leveldb/db.h>
+#include <lmdb.h>
@@
-    leveldb::DB* pdb;
+    MDB_env* m_env;
+    MDB_dbi m_dbi;
```

## Validation

Describe how the patch was validated.

Examples:
- `./autogen.sh`
- `./configure --without-gui --disable-tests --disable-bench`
- `make -j$(nproc)`
- smoke test for DB read/write/iterator operations

## Checklist

- [ ] Build succeeds on Linux
- [ ] Build succeeds on macOS
- [ ] Build succeeds on Windows
- [ ] DB read/write path still works
- [ ] Iterator semantics preserved
- [ ] Serialization remains compatible
- [ ] Release artifacts produced per platform

## Notes

Add any deployment, compatibility, or follow-up notes here.
