// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2018-2019 The bitpcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <limits>

#include <boost/assign/list_of.hpp>



using namespace std;
using namespace boost::assign;

static bool regenerate = false;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
        (0, uint256("00000c7a874a74e7c5ea075affa113fbe39a8ecf8897d92254f4749735d223c6"))
    ;

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1563120000, // * UNIX timestamp of last checkpoint block
    1,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    3000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet = boost::assign::map_list_of(0, uint256("0"));
static const Checkpoints::CCheckpointData dataTestnet = {&mapCheckpointsTestnet, 1541462411, 0, 250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest = boost::assign::map_list_of(0, uint256("0"));
static const Checkpoints::CCheckpointData dataRegtest = {&mapCheckpointsRegtest, 0, 0, 0};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xb9;
        pchMessageStart[1] = 0xb4;
        pchMessageStart[2] = 0x2e;
        pchMessageStart[3] = 0x2e;

        nDefaultPort = 11120;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        bnStartWork = ~uint256(0) >> 24;

        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;  // 1 minute
        nTargetSpacingSlowLaunch = 1 * 60; // before block 100
        nPoSTargetSpacing = 60;  // 1 minute
        nMaturity = 120;
        nMasternodeCountDrift = 3;
        nMaxMoneyOut = 197000000  * COIN;
        nStartMasternodePaymentsBlock = 120;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 301;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();

        const char* pszTimestamp = "Cashollar lunch for 2019/07 - CSO ";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04c14b8bf5aa978df3a232550f9c55409fa41d9227e76708700ec8a4f95ad0f3406753e6987635caa3b1d2cf7db6aa3974552ae7c2c7c46eec8fa074e92d1c5d3c") << OP_CHECKSIG;
        txNew.blob = "Genesis Tx";
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1563120000; // 2019-07-15 T00:00:00+00:00
        genesis.nBits = 0x1e0ffff0; // 504365040
        genesis.nNonce = 944688;
		hashGenesisBlock = genesis.GetHash();
		
		if (regenerate) {
            hashGenesisBlock = uint256S("");
            genesis.nNonce = 0;
            if (true && (genesis.GetHash() != hashGenesisBlock)) {
                while (genesis.GetHash() > bnProofOfWorkLimit)
                {
                    ++genesis.nNonce;
                    if (genesis.nNonce == 0)
                    {
                        ++genesis.nTime;
                    }
                }
                std::cout << "// Mainnet ---";
                std::cout << " nonce: " << genesis.nNonce;
                std::cout << " time: " << genesis.nTime;
                std::cout << " hash: 0x" << genesis.GetHash().ToString().c_str();
                std::cout << " merklehash: 0x"  << genesis.hashMerkleRoot.ToString().c_str() <<  "\n";
            }
        } else {
            assert(hashGenesisBlock == uint256("0x00000c7a874a74e7c5ea075affa113fbe39a8ecf8897d92254f4749735d223c6"));
            assert(genesis.hashMerkleRoot == uint256("0x90a135ccbfd730ddee8079bc595bf51bb33b5bef5b5537e0761ab3ef79ec0d87"));
        }
		

        
        //vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("seed1.bitpb.top", "seed21.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed2.bitpb.top", "seed22.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed3.bitpb.top", "seed23.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed4.bitpb.top", "seed24.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed5.bitpb.top", "seed25.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed6.bitpb.top", "seed26.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed7.bitpb.top", "seed27.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed8.bitpb.top", "seed28.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed9.bitpb.top", "seed29.bitpb.top"));
        vSeeds.push_back(CDNSSeedData("seed10.bitpb.top", "seed30.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed11.bitpb.top", "seed31.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed12.bitpb.top", "seed32.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed13.bitpb.top", "seed33.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed14.bitpb.top", "seed34.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed15.bitpb.top", "seed35.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed16.bitpb.top", "seed36.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed17.bitpb.top", "seed37.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed18.bitpb.top", "seed38.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed19.bitpb.top", "seed39.bitpb.top"));
		vSeeds.push_back(CDNSSeedData("seed20.bitpb.top", "seed40.bitpb.top"));
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 0); // C
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 63); // S
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 0); // 1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 9984
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x04)(0x61).convert_to_container<std::vector<unsigned char> >(); // 1121

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;

        nStakeInputMin = 10 * COIN;
        strDevFeeAddress = "16YN4Wr9cTzrB1v22kU7ZW3PbKm3TanD8q";

        vAlertPubKey = ParseHex("0428e89226dd86459df40d436a067c83749c78d653e22c556ae2d9b322296f3f1604e2f4789128386bc4acd6184c9a0062cf0cb98cf71cdbca1e808c25b7670367");
        vGMPubKey = ParseHex("049e20bd6cc0da7270bfa60daf381593377418ce9270b7dd38a93026acae98966e89da65067b41e388e194a7e4e2276336b3ddba5e3d5bbc81a78a04f982dfb4fc");
        strSporkKey = "04d45416e4a64b1b051e2a2ebd80ced5efe148cf5fbcb70e56860957675a2da1a21fd522c42c1ed18a1ec42641589a09cf3f58678d213825dc21798183a005a984";
        strObfuscationPoolDummyAddress = "16YN4Wr9cTzrB1v22kU7ZW3PbKm3TanD8q";

    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x47;
        pchMessageStart[1] = 0x77;
        pchMessageStart[2] = 0x66;
        pchMessageStart[3] = 0xbb;

        bnProofOfWorkLimit = ~uint256(0) >> 1;
        bnStartWork = bnProofOfWorkLimit;

        nDefaultPort = 42322;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;  // 1 minute
//        nLastPOWBlock = std::numeric_limits<decltype(nLastPOWBlock)>::max();
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();
        nMaxMoneyOut = 1000000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1546300800;
        genesis.nNonce = 0;

        hashGenesisBlock = genesis.GetHash();

        //assert(hashGenesisBlock == uint256("019a701040d795514ea77eda681e74f8de73afdb1b39d541fc0c697585b878dc"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 137); // Testnet bitpcoin addresses start with 'x'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet bitpcoin script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        nStakeInputMin = 1 * COIN;
        strDevFeeAddress = "xJETLzAQWJj18aQ74cHqAtdStrZves2U4A";

        vAlertPubKey = ParseHex("04e2a902b30e8e5430e4f3d1ac79630282cc65a036d0aa70ec041d8903b9a626b601a888d8479412bcc363250b02cb2f0e783e7dbeef8606a6ab635fde952949f9");
        vGMPubKey = ParseHex("0414b78fd29848ca55bacabe49c6bf53c8cb5224cdd84590f21616457c564b01d2c26c69fea8a55b5e336cb40981ba3167b04ddd149a21f59ab07cf30a4b7285b1");
        strSporkKey = "043f305881c14698ca11d9ccbbef49714a816da377bcc0b25d2d54e5a5b266605353e5ec4c7f9958899b5e7a550225d652151ada50d040277ef75ada1214f92e77";
        strObfuscationPoolDummyAddress = "xJR9MjNhPLKLLCowMWNznC9gkEQHQPjcJr";

    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;

        bnStartWork = ~uint256(0) >> 20;

        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetSpacing = 1 * 60;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1541462422;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 1;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 52322;

        //assert(hashGenesisBlock == uint256("300552a9db8b2921c3c07e5bbf8694df5099db579742e243daeaf5008b1e74de"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fMineBlocksOnDemand = true;


    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
