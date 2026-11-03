// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GLOBALBOOST_WALLET_MLDSA_WALLETDB_H
#define GLOBALBOOST_WALLET_MLDSA_WALLETDB_H

#include <key_mldsa.h>

#include <map>
#include <string>
#include <vector>

namespace wallet {

struct MldsaWalletEntry {
    std::string key_name;
    std::vector<unsigned char> pubkey;
    std::vector<unsigned char> seckey;
};

class CMldsaWalletDB
{
public:
    bool Load();
    bool Save();
    bool AddKey(const std::string& name, const CMldsaPublicKey& pubkey, const CMldsaSecretKey& seckey);
    bool RemoveKey(const std::string& name);
    std::vector<MldsaWalletEntry> ListKeys() const;

private:
    std::map<std::string, MldsaWalletEntry> m_entries;
};

} // namespace wallet

#endif // GLOBALBOOST_WALLET_MLDSA_WALLETDB_H
