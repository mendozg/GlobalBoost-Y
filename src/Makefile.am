# Copyright (c) 2013-2016 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# Pattern rule to print variables, e.g. make print-top_srcdir
print-%: FORCE
	@echo '$*'='$($*)'

DIST_SUBDIRS = secp256k1

AM_LDFLAGS = $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS) $(GPROF_LDFLAGS) $(SANITIZER_LDFLAGS) $(LTO_LDFLAGS) $(CORE_LDFLAGS)
AM_CXXFLAGS = $(DEBUG_CXXFLAGS) $(HARDENED_CXXFLAGS) $(WARN_CXXFLAGS) $(NOWARN_CXXFLAGS) $(ERROR_CXXFLAGS) $(GPROF_CXXFLAGS) $(SANITIZER_CXXFLAGS) $(LTO_CXXFLAGS) $(CORE_CXXFLAGS)
AM_CPPFLAGS = $(DEBUG_CPPFLAGS) $(HARDENED_CPPFLAGS) $(CORE_CPPFLAGS)
AM_LIBTOOLFLAGS = --preserve-dup-deps
PTHREAD_FLAGS = $(PTHREAD_CFLAGS) $(PTHREAD_LIBS)
EXTRA_LIBRARIES =

lib_LTLIBRARIES =
noinst_LTLIBRARIES =

bin_PROGRAMS =
noinst_PROGRAMS =
check_PROGRAMS =
TESTS =
BENCHMARKS =

GLOBALBOOST_INCLUDES=-I$(builddir) -I$(srcdir)/$(MINISKETCH_INCLUDE_DIR_INT) -I$(srcdir)/secp256k1/include -I$(srcdir)/$(UNIVALUE_INCLUDE_DIR_INT) $(LEVELDB_CPPFLAGS)

LIBGLOBALBOOST_NODE=libglobalboost_node.a
LIBGLOBALBOOST_COMMON=libglobalboost_common.a
LIBGLOBALBOOST_CONSENSUS=libglobalboost_consensus.a
LIBGLOBALBOOST_CLI=libglobalboost_cli.a
LIBGLOBALBOOST_UTIL=libglobalboost_util.a
LIBGLOBALBOOST_CRYPTO_BASE=crypto/libglobalboost_crypto_base.la
LIBGLOBALBOOSTQT=qt/libglobalboostqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBGLOBALBOOST_ZMQ=libglobalboost_zmq.a
endif
if BUILD_GLOBALBOOST_LIBS
LIBGLOBALBOOSTCONSENSUS=libglobalboostconsensus.la
endif
if BUILD_GLOBALBOOST_KERNEL_LIB
LIBGLOBALBOOSTKERNEL=libglobalboostkernel.la
endif
if ENABLE_WALLET
LIBGLOBALBOOST_WALLET=libglobalboost_wallet.a
LIBGLOBALBOOST_WALLET_TOOL=libglobalboost_wallet_tool.a
endif

include Makefile.qttest.include

.PHONY: FORCE check-symbols check-security
# Ensure all dependencies are built before we use them to build more things
$(LIBSECP256K1): FORCE
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C secp256k1 libsecp256k1.la

# Pattern rule to print variables, e.g. make print-top_srcdir

GLOBALBOOST_CORE_H = \
  addrdb.h \
  addrman.h \
  addrman_impl.h \
  attributes.h \
  banman.h \
  base58.h \
  bech32.h \
  bloom.h \
  blockencodings.h \
  blockfilter.h \
  blockfilter.h \
  blockindexstats.h \
  blockindexstats.h \
  blockindex.h \
  blockindex.h \
  block.h \
  blockstorage.h \
  blockstorage.h \
  blockvalidation.h \
  blockvalidation.h \
  block_index.h \
  blockvalidator.h \
  blockvalidator.h \
  cachemultimap.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coins.h \
  coinscache.h \
  coinscache.h \
  common/args.h \
  common/bloom.h \
  common/init.h \
  common/messages.h \
  common/run_tests.h \
  common/settings.h \
  common/signmessage.h \
  common/system.h \
  common/url.h \
  compat.h \
  compat/compat.h \
  compressor.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  core_io.h \
  core_memusage.h \
  cuckoocache.h \
  ctpl/ctpl_stl.h \
  ctpl/ctpl.h \
  ctpl/ctpl.h.swp \
  database.h \
  dbwrapper.h \
  deploymentinfo.h \
  deploymentstatus.h \
  deserialize.h \
  dummywallet.h \
  eventmanager.h \
  events.h \
  events.h \
  flatfile.h \
  flatfile.h \
  flat_db.h \
  flat_db_sqlite.h \
  flat_db_leveldb.h \
  flatdataset.h \
  flat_dataset.h \
  flushablestorage.h \
  fs.h \
  ftl/concurrent_vector.h \
  ftl/concurrent_queue.h \
  ftl/event.h \
  ftl/object_pool.h \
  ftl/lfqueue.h \
  ftl/vector.h \
  ftl/deque.h \
  ftl/task_system.h \
  ftl/task_counter.h \
  ftl/atomic.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  i2p.h \
  index/base.h \
  index/blockfilterindex.h \
  index/coinstatsindex.h \
  index/disktxpos.h \
  index/disktxpos.h \
  indirectmap.h \
  init.h \
  init/common.h \
  init/globalboostd.h \
  interfaces/chain.h \
  interfaces/echo.h \
  interfaces/handler.h \
  interfaces/init.h \
  interfaces/node.h \
  interfaces/wallet.h \
  i2p.h \
  kernel/blockmanager.h \
  kernel/chainparams.h \
  kernel/checks.h \
  kernel/coinstats.h \
  kernel/context.h \
  kernel/mempool_limits.h \
  kernel/mempool_persist.h \
  kernel/mempool_removal_reason.h \
  kernel/mempool_removal_reason.h \
  kernel/mempool_removal_reason.h \
  kernel/mempool_removal_reason.h \
  kernel/mempool.h \
  kernel/signals.h \
  kernel/validation.h \
  kernel/warnings.h \
  key.h \
  key_io.h \
  logging.h \
  logging/timer.h \
  mapfile.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  modules/mp_node_rpc.h \
  modules/mp_network_config.h \
  modules/mp_networkmanager.h \
  modules/mp_p2p_message_handler.h \
  modules/mp_message_router.h \
  net.h \
  netaddress.h \
  netbase.h \
  netmessagemaker.h \
  network.h \
  node/blockstorage.h \
  node/caches.h \
  node/chainstate.h \
  node/coin_selection.h \
  node/coins_view.h \
  node/connection_types.h \
  node/context.h \
  node/eviction.h \
  node/mini_node.h \
  node/psbt.h \
  node/transaction_metadata.h \
  node/tx_memory_pool.h \
  node/txdownloadman.h \
  node/txdownloadman_impl.h \
  node/validation.h \
  noui.h \
  optional.h \
  outputtype.h \
  p2p_message_type.h \
  p2p/address_info.h \
  p2p/banman.h \
  p2p/connection_types.h \
  p2p/net.h \
  p2p/netbase.h \
  p2p/netmessagemaker.h \
  p2p/protocol.h \
  p2p/protocol_types.h \
  p2p_message_handler.h \
  p2p_message_router.h \
  p2p_network_config.h \
  p2p_networkmanager.h \
  p2p_protocol.h \
  p2p_protocol_types.h \
  paymentserver.h \
  peerman.h \
  peerman.h \
  peerman.h \
  policy/feerate.h \
  policy/fees.h \
  policy/packages.h \
  policy/policy.h \
  policy/rbf.h \
  policy/settings.h \
  pow.h \
  primitives/block.h \
  primitives/block.h \
  primitives/block.h \
  primitives/transaction.h \
  primitives/txid.h \
  primitives/txid.h \
  protocol.h \
  psbt.h \
  pubkey.h \
  random.h \
  randomenv.h \
  raw_transaction.h \
  rpc/blockchain.h \
  rpc/blockchain_json.h \
  rpc/client.h \
  rpc/command.h \
  rpc/connection_info.h \
  rpc/debug.h \
  rpc/describe.h \
  rpc/external_signer.h \
  rpc/fee_estimate.h \
  rpc/jsonrpc.h \
  rpc/mempool.h \
  rpc/mempool_fee_estimate.h \
  rpc/misc.h \
  rpc/moneypot.h \
  rpc/net.h \
  rpc/peer_connection.h \
  rpc/protocol.h \
  rpc/rawtransaction.h \
  rpc/rawtransaction_util.h \
  rpc/server.h \
  rpc/signmessage.h \
  rpc/signrawtransaction.h \
  rpc/system.h \
  rpc/util.h \
  rpc/wallet_util.h \
  script/descriptor.h \
  script/keyorigin.h \
  script/script.h \
  script/scripttype.h \
  script/sign.h \
  script/signingprovider.h \
  script/solidity_utils.h \
  script/standard.h \
  serialize.h \
  server.h \
  settings.h \
  shutdown.h \
  signet.h \
  skiplist.h \
  socket.h \
  span.h \
  spork.h \
  stacktraces.h \
  streams.h \
  support/allocators/mt_pooled_secure.h \
  support/allocators/pooled_secure.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/lockedpool.h \
  sync.h \
  syncstatus.h \
  system.h \
  telemetry.h \
  test/fuzz/fuzz.h \
  test/fuzz/util.h \
  test/util/blockfilter.h \
  test/util/index.h \
  test/util/mining.h \
  test/util/net.h \
  test/util/script.h \
  test/util/setup_common.h \
  test/util/str.h \
  test/util/transaction.h \
  test/util/txmempool.h \
  test/util/validation.h \
  test/util/wallet.h \
  threadinterrupt.h \
  threadsafety.h \
  threadutil.h \
  time.h \
  timedata.h \
  torcontrol.h \
  torexitaddresses.h \
  totalcoins.h \
  tr.h \
  txdb.h \
  txdb.h \
  txglobaldata.h \
  txglobaldata.h \
  txmempool.h \
  txmempool.h \
  txmempool.h \
  txmemorybuffer.h \
  txmemorybuffer.h \
  txmemorybuffer.h \
  txmessages.h \
  txorphanpool.h \
  txorphanpool.h \
  txpool.h \
  txpool.h \
  txpool.h \
  txpool.h \
  txpool.h \
  txpoolmgr.h \
  txpoolmgr.h \
  txpoolmgr.h \
  txpoolmgr.h \
  txpoolmgr.h \
  txpoolmgr_impl.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txpoolstats.h \
  txqueue.h \
  txrelay.h \
  txrelay.h \
  txrelay.h \
  txrelay.h \
  txrelay.h \
  txrelay.h \
  txspeedometer.h \
  txvalidationstate.h \
  txvalidationstate.h \
  txvalidationstate.h \
  txvalidationstate.h \
  txvalidationstate.h \
  txvalidationstate.h \
  txvalidationstate.h \
  txxmempool_manager.h \
  txxmempool_manager.h \
  txxmempool_manager.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  txvault.h \
  ui_interface.h \
  undo.h \
  unordered_lru_cache.h \
  util/asmap.h \
  util/bip32path.h \
  util/bytevectorhash.h \
  util/check.h \
  util/epochguard.h \
  util/exception.h \
  util/fastrange.h \
  util/fees.h \
  util/getuniquepath.h \
  util/golombrice.h \
  util/hash.h \
  util/hashwrapper.h \
  util/hasher.h \
  util/macros.h \
  util/message.h \
  util/moneystr.h \
  util/overrides.h \
  util/ranges.h \
  util/readwritefile.h \
  util/result.h \
  util/serfloat.h \
  util/serialize.h \
  util/sock.h \
  util/spanparsing.h \
  util/strencodings.h \
  util/string.h \
  util/system.h \
  util/time.h \
  util/transaction.h \
  util/translation.h \
  util/types.h \
  util/url.h \
  util/vector.h \
  util/vector_hash.h \
  validation.h \
  validationinterface.h \
  validationinterface.h \
  validationinterface.h \
  validationinterface.h \
  validationinterface.h \
  validationinterface.h \
  validationstate.h \
  validationstate.h \
  versionbitsinfo.h \
  wallet/bdb.h \
  wallet/coincontrol.h \
  wallet/coinselection.h \
  wallet/context.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/dump.h \
  wallet/external_signer_scriptpubkeyman.h \
  wallet/feebumper.h \
  wallet/fees.h \
  wallet/ismine.h \
  wallet/load.h \
  wallet/rpc/util.h \
  wallet/salvage.h \
  wallet/scriptpubkeyman.h \
  wallet/sqlite.h \
  wallet/transaction.h \
  wallet/types.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  wallet/walletutil.h \
  walletdb.h \
  walletinitflags.h \
  walletmodel.h \
  walletmodel.h \
  walletview.h \
  walletview.h \
  warnings.h \
  worker.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h \
  zmq/zmqrpc.h \
  zmq/zmqutil.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libglobalboost_util_a-clientversion.$(OBJEXT): obj/build.h

# server #
libglobalboost_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(GLOBALBOOST_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CPPFLAGS) $(EVENT_PTHREADS_CPPFLAGS)
libglobalboost_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libglobalboost_server_a_SOURCES = \
  addrdb.cpp \
  addrman.cpp \
  banman.cpp \
  blockfilter.cpp \
  blockindexstats.cpp \
  blockindex.cpp \
  blockstorage.cpp \
  blockvalidation.cpp \
  block_index.cpp \
  blockvalidator.cpp \
  chain.cpp \
  checkpoints.cpp \
  consensus/tx_verify.cpp \
  consensus/validation.h \
  dbwrapper.cpp \
  deploymentinfo.cpp \
  deploymentstatus.cpp \
  eventmanager.cpp \
  events.cpp \
  flatfile.cpp \
  flat_db.cpp \
  flat_db_sqlite.cpp \
  flat_db_leveldb.cpp \
  flatdataset.cpp \
  flushablestorage.cpp \
  i2p.cpp \
  index/base.cpp \
  index/blockfilterindex.cpp \
  index/coinstatsindex.cpp \
  init.cpp \
  init/common.cpp \
  init/globalboostd.cpp \
  kernel/blockmanager.cpp \
  kernel/chainparams.cpp \
  kernel/checks.cpp \
  kernel/coinstats.cpp \
  kernel/context.cpp \
  kernel/mempool.cpp \
  kernel/mempool_persist.cpp \
  kernel/signals.cpp \
  kernel/validation.cpp \
  kernel/warnings.cpp \
  logging.cpp \
  logging/timer.cpp \
  mapfile.cpp \
  miner.cpp \
  modules/mp_node_rpc.cpp \
  modules/mp_network_config.cpp \
  modules/mp_networkmanager.cpp \
  modules/mp_p2p_message_handler.cpp \
  modules/mp_message_router.cpp \
  net.cpp \
  netaddress.cpp \
  netbase.cpp \
  netmessagemaker.cpp \
  node/blockstorage.cpp \
  node/caches.cpp \
  node/chainstate.cpp \
  node/context.cpp \
  node/eviction.cpp \
  node/mini_node.cpp \
  node/psbt.cpp \
  node/tx_memory_pool.cpp \
  node/txdownloadman.cpp \
  node/validation.cpp \
  noui.cpp \
  p2p_message_handler.cpp \
  p2p_message_router.cpp \
  p2p_network_config.cpp \
  p2p_networkmanager.cpp \
  policy/feerate.cpp \
  policy/fees.cpp \
  policy/packages.cpp \
  policy/policy.cpp \
  policy/rbf.cpp \
  policy/settings.cpp \
  pow.cpp \
  rpc/blockchain.cpp \
  rpc/blockchain_json.cpp \
  rpc/command.cpp \
  rpc/connection_info.cpp \
  rpc/debug.cpp \
  rpc/describe.cpp \
  rpc/external_signer.cpp \
  rpc/fee_estimate.cpp \
  rpc/jsonrpc.cpp \
  rpc/mempool.cpp \
  rpc/mempool_fee_estimate.cpp \
  rpc/misc.cpp \
  rpc/moneypot.cpp \
  rpc/net.cpp \
  rpc/peer_connection.cpp \
  rpc/protocol.cpp \
  rpc/rawtransaction.cpp \
  rpc/rawtransaction_util.cpp \
  rpc/server.cpp \
  rpc/signmessage.cpp \
  rpc/signrawtransaction.cpp \
  rpc/system.cpp \
  rpc/util.cpp \
  signet.cpp \
  spork.cpp \
  syncstatus.cpp \
  telemetry.cpp \
  torcontrol.cpp \
  torexitaddresses.cpp \
  txdb.cpp \
  txglobaldata.cpp \
  txmempool.cpp \
  txmemorybuffer.cpp \
  txmessages.cpp \
  txorphanpool.cpp \
  txpool.cpp \
  txpoolmgr.cpp \
  txpoolstats.cpp \
  txqueue.cpp \
  txrelay.cpp \
  txspeedometer.cpp \
  txvalidationstate.cpp \
  txvault.cpp \
  validation.cpp \
  validationinterface.cpp \
  validationstate.cpp \
  warnings.cpp \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqconfig.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp \
  zmq/zmqrpc.cpp \
  zmq/zmqutil.cpp \
  $(GLOBALBOOST_CORE_H)

#

# wallet #
if ENABLE_WALLET

libglobalboost_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(GLOBALBOOST_INCLUDES) $(BOOST_CPPFLAGS) $(BDB_CPPFLAGS) $(SQLITE_CPPFLAGS)
libglobalboost_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libglobalboost_wallet_a_SOURCES = \
  interfaces/wallet.cpp \
  wallet/bdb.cpp \
  wallet/coincontrol.cpp \
  wallet/coinselection.cpp \
  wallet/context.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/dump.cpp \
  wallet/external_signer_scriptpubkeyman.cpp \
  wallet/feebumper.cpp \
  wallet/fees.cpp \
  wallet/ismine.cpp \
  wallet/load.cpp \
  wallet/rpc/backup.cpp \
  wallet/rpc/encrypt.cpp \
  wallet/rpc/spend.cpp \
  wallet/rpc/transactions.cpp \
  wallet/rpc/util.cpp \
  wallet/salvage.cpp \
  wallet/scriptpubkeyman.cpp \
  wallet/sqlite.cpp \
  wallet/transaction.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  wallet/walletutil.cpp \
  $(GLOBALBOOST_CORE_H)

libglobalboost_wallet_tool_a_CPPFLAGS = $(AM_CPPFLAGS) $(GLOBALBOOST_INCLUDES) $(BOOST_CPPFLAGS)
libglobalboost_wallet_tool_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libglobalboost_wallet_tool_a_SOURCES = \
  wallet/wallettool.cpp \
  $(GLOBALBOOST_CORE_H)
#

# crypto #
crypto_libglobalboost_crypto_base_la_CPPFLAGS = $(AM_CPPFLAGS)

# Specify -static in both CXXFLAGS and LDFLAGS so libtool will only build a
# static version of this library. We don't need a dynamic version, and a dynamic
# version can't be used on windows anyway because the library doesn't currently
# export DLL symbols.
crypto_libglobalboost_crypto_base_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libglobalboost_crypto_base_la_LDFLAGS = $(AM_LDFLAGS) -static

crypto_libglobalboost_crypto_base_la_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/chacha_poly_aead.h \
  crypto/chacha_poly_aead.cpp \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/common.h \
  crypto/hkdf_sha256_32.cpp \
  crypto/hkdf_sha256_32.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/poly1305.h \
  crypto/poly1305.cpp \
  crypto/muhash.h \
  crypto/muhash.cpp \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha3.cpp \
  crypto/sha3.h \
  crypto/sha512.cpp \
  crypto/sha512.h \
  crypto/siphash.cpp \
  crypto/siphash.h \
  crypto/yescrypt/yescrypt.c \
  crypto/yescrypt/yescrypt_cpp.cpp \
  crypto/yescrypt/yescrypt_cpp.h

noinst_HEADERS = \
  crypto/yescrypt/sha256.h \
  crypto/yescrypt/sha256_c.h \
  crypto/yescrypt/sysendian.h \
  crypto/yescrypt/yescrypt.h \
  crypto/yescrypt/yescrypt-best_c.h \
  crypto/yescrypt/yescrypt-opt_c.h \
  crypto/yescrypt/yescrypt-platform_c.h \
  crypto/yescrypt/yescrypt-simd_c.h


if USE_ASM
crypto_libglobalboost_crypto_base_la_SOURCES += crypto/sha256_sse4.cpp
endif

# See explanation for -static in crypto_libglobalboost_crypto_base_la's LDFLAGS and
# CXXFLAGS above
crypto_libglobalboost_crypto_sse41_la_LDFLAGS = $(AM_LDFLAGS) -static
crypto_libglobalboost_crypto_sse41_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libglobalboost_crypto_sse41_la_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libglobalboost_crypto_sse41_la_CXXFLAGS += $(SSE41_CXXFLAGS)
crypto_libglobalboost_crypto_sse41_la_CPPFLAGS += -DENABLE_SSE41
crypto_libglobalboost_crypto_sse41_la_SOURCES = crypto/sha256_sse41.cpp

# See explanation for -static in crypto_libglobalboost_crypto_base_la's LDFLAGS and
# CXXFLAGS above
crypto_libglobalboost_crypto_avx2_la_LDFLAGS = $(AM_LDFLAGS) -static
crypto_libglobalboost_crypto_avx2_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libglobalboost_crypto_avx2_la_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libglobalboost_crypto_avx2_la_CXXFLAGS += $(AVX2_CXXFLAGS)
crypto_libglobalboost_crypto_avx2_la_CPPFLAGS += -DENABLE_AVX2
crypto_libglobalboost_crypto_avx2_la_SOURCES = crypto/sha256_avx2.cpp

noinst_LTLIBRARIES += $(LIBGLOBALBOOST_CRYPTO_BASE)

noinst_LTLIBRARIES += crypto/libglobalboost_crypto_sse41.la
noinst_LTLIBRARIES += crypto/libglobalboost_crypto_avx2.la

libglobalboost_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(GLOBALBOOST_INCLUDES)
libglobalboost_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libglobalboost_common_a_SOURCES = \
  base58.cpp \
  bech32.cpp \
  bloom.cpp \
  chainparams.cpp \
  coins.cpp \
  coinscache.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  key.cpp \
  key_io.cpp \
  keystore.cpp \
  message.cpp \
  netaddress.cpp \
  netbase.cpp \
  net_permissions.cpp \
  outputtype.cpp \
  policy/feerate.cpp \
  protocol.cpp \
  psbt.cpp \
  rpc/rawtransaction_util.cpp \
  rpc/util.cpp \
  script/descriptor.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/scripttype.cpp \
  script/sign.cpp \
  script/signingprovider.cpp \
  script/standard.cpp \
  script/solidity_utils.cpp \
  versionbitsinfo.cpp \
  $(GLOBALBOOST_CORE_H)

libglobalboost_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(GLOBALBOOST_INCLUDES)
libglobalboost_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libglobalboost_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/amount.h \
  consensus/checks.cpp \
  consensus/consensus.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/tx_verify.cpp \
  consensus/tx_verify.h \
  consensus/validation.h \
  crypto/hash.cpp \
  crypto/hash.h \
  hash.h \
  primitives/block.h \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/scripttype.h \
  serialize.h \
  span.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  util/strencodings.cpp \
  util/strencodings.h

libglobalboost_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(GLOBALBOOST_INCLUDES)
libglobalboost_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libglobalboost_util_a_SOURCES = \
  support/allocators/mt_pooled_secure.cpp \
  support/allocators/mt_pooled_secure.h \
  support/allocators/pooled_secure.cpp \
  support/allocators/pooled_secure.h \
  support/allocators/secure.cpp \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.cpp \
  support/cleanse.h \
  support/events.cpp \
  support/events.h \
  support/lockedpool.cpp \
  support/lockedpool.h \
  clientversion.cpp \
  common/args.cpp \
  common/args.h \
  common/init.cpp \
  common/init.h \
  common/messages.cpp \
  common/messages.h \
  common/run_tests.cpp \
  common/run_tests.h \
  common/settings.cpp \
  common/settings.h \
  common/signmessage.cpp \
  common/signmessage.h \
  common/system.cpp \
  common/system.h \
  common/url.cpp \
  common/url.h \
  compat/compat.h \
  compat/glibcxx_sanity_test.cpp \
  compat/strnlen.cpp \
  fs.cpp \
  fs.h \
  logging.cpp \
  logging.h \
  logging/timer.cpp \
  logging/timer.h \
  random.cpp \
  random.h \
  randomenv.cpp \
  randomenv.h \
  serialize.h \
  stacktraces.cpp \
  stacktraces.h \
  support/cleanse.cpp \
  support/cleanse.h \
  sync.cpp \
  sync.h \
  threadinterrupt.cpp \
  threadinterrupt.h \
  threadsafety.h \
  threadutil.cpp \
  threadutil.h \
  time.cpp \
  time.h \
  timedata.cpp \
  timedata.h \
  util/asmap.cpp \
  util/asmap.h \
  util/bip32path.cpp \
  util/bip32path.h \
  util/bytevectorhash.h \
  util/check.cpp \
  util/check.h \
  util/epochguard.h \
  util/exception.h \
  util/fastrange.h \
  util/fees.cpp \
  util/fees.h \
  util/getuniquepath.h \
  util/golombrice.h \
  util/hash.cpp \
  util/hash.h \
  util/hashwrapper.h \
  util/hasher.h \
  util/macros.h \
  util/message.cpp \
  util/message.h \
  util/moneystr.cpp \
  util/moneystr.h \
  util/overrides.h \
  util/ranges.h \
  util/readwritefile.cpp \
  util/readwritefile.h \
  util/result.h \
  util/serfloat.h \
  util/serialize.h \
  util/sock.cpp \
  util/sock.h \
  util/spanparsing.cpp \
  util/spanparsing.h \
  util/strencodings.cpp \
  util/strencodings.h \
  util/string.cpp \
  util/string.h \
  util/system.cpp \
  util/system.h \
  util/time.cpp \
  util/time.h \
  util/transaction.cpp \
  util/transaction.h \
  util/translation.h \
  util/types.h \
  util/url.cpp \
  util/url.h \
  util/vector.h \
  util/vector_hash.h \
  $(GLOBALBOOST_CORE_H)

libglobalboost_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(GLOBALBOOST_INCLUDES)
libglobalboost_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libglobalboost_cli_a_SOURCES = \
  rpc/client.cpp \
  rpc/client.h \
  $(GLOBALBOOST_CORE_H)

nodist_libglobalboost_util_a_SOURCES = $(srcdir)/obj/build.h
#

libglobalboost_consensus_a_SOURCES += \
  crypto/sha256.cpp \
  crypto/sha256.h

#

include Makefile.leveldb.include
#

if ENABLE_QT
include Makefile.qt.include
endif
#

if ENABLE_TESTS
include Makefile.test.include
endif
#

if ENABLE_BENCH
include Makefile.bench.include
endif
#

if ENABLE_FUZZ_BINARY
include Makefile.fuzz.include
endif

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/glibc_sanity_test.raw compat/glibc_sanity_test
CLEANFILES += obj/build.h

EXTRA_DIST = Makefile.leveldb.include Makefile.test.include Makefile.bench.include Makefile.qt.include Makefile.qttest.include Makefile.fuzz.include


distclean-local:
	-$(MAKE) -C secp256k1 distclean
	-rm -f secp256k1/libsecp256k1.la secp256k1/libsecp256k1.pc

distclean-local: clean-docs

clean-local:
	-$(MAKE) -C secp256k1 clean

.rc.o:
	@test -f $(RC) || (echo "windres not found, but is required to compile bitcoin core for windows. Terminating build."; exit 1)
	$(AM_V_GEN) $(RC) -DWINDRES_PREPROC -i $< -o $@
.res.o:
	@test -f $(RC) || (echo "windres not found, but is required to compile bitcoin core for windows. Terminating build."; exit 1)
	$(AM_V_GEN) $(RC) -DWINDRES_PREPROC -i $< -o $@

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC) || (echo "protoc not found, but is required to compile bitcoin core. Terminating build."; exit 1)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --python_out=$(@D) $<

include Makefile.leveldb.include
