// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GLOBALBOOST_KEY_MLDSA_ADDRESS_H
#define GLOBALBOOST_KEY_MLDSA_ADDRESS_H

#include <key_mldsa.h>

#include <string>
#include <vector>

namespace wallet {

class CMldsaAddress
{
public:
    explicit CMldsaAddress(const CMldsaPublicKey& pubkey);
    CMldsaAddress() = default;

    bool IsValid() const;
    std::string ToString() const;
    bool FromString(const std::string& str);

    CMldsaPublicKey GetPubKey() const;
    void SetPubKey(const CMldsaPublicKey& pubkey);

private:
    CMldsaPublicKey m_pubkey;
};

} // namespace wallet

#endif // GLOBALBOOST_KEY_MLDSA_ADDRESS_H
