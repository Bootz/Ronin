/***
 * Demonstrike Core
 */

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////
// Fire Locations
//////////////////////////////////////////////////////////////////////////

const AVSpawnLocation g_fireLocations[AV_NUM_CONTROL_POINTS][30] =
{
    { { 0.0f, 0.0f, 0.0f }, },  // Aid Station
    { { 0.0f, 0.0f, 0.0f }, },  // Stormpike Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Stonehearth Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Snowfall Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Coldtooth Mine
    { { 0.0f, 0.0f, 0.0f }, },  // Irondeep Mine
    { { 0.0f, 0.0f, 0.0f }, },  // Iceblood Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Frostwolf Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Frostwolf Relief
    // Dun Baldar North
    { { 667.917297f, -137.020660f, 63.645058f, 1.094849f }, { 680.675903f, -125.554802f, 63.666199f, 0.089540f },
    { 685.923828f, -148.394562f, 56.618328f, 1.138047f }, { 667.368164f, -147.642548f, 56.621113f, 4.523115f },
    { 666.998413f, -143.095612f, 49.673607f, 4.122564f }, { 676.130615f, -123.653824f, 49.672577f, 2.744190f },
    { 666.790649f, -127.394073f, 49.638577f, 3.348947f }, { 0.0f, 0.0f, 0.0f }, },
    // Dun Baldar South
    { { 568.422852f, -83.935478f, 51.942871f, 2.324003f }, { 547.981018f, -93.095024f, 51.942841f, 0.898504f },
    { 549.647034f, -80.551971f, 44.820171f, 2.858073f }, { 552.589355f, -96.653267f, 44.819138f, 0.619688f },
    { 562.575439f, -78.773170f, 37.949261f, 1.644633f }, { 568.352722f, -92.772797f, 37.949261f, 0.761060f },
    { 575.015137f, -78.856613f, 37.921127f, 0.596126f }, { 0.0f, 0.0f, 0.0f }, },
    // Icewing
    { { 198.799316f, -361.116241f, 56.391476f, 5.363494f }, { 217.211273f, -375.348236f, 56.368317f, 0.494023f },
    { 200.838547f, -362.912231f, 49.267738f, 2.708846f }, { 208.172836f, -379.141357f, 49.267738f, 5.826873f },
    { 209.692886f, -356.993683f, 42.398838f, 2.335778f }, { 223.358307f, -374.435516f, 42.396271f, 5.288878f },
    { 222.380539f, -357.525726f, 42.361271f, 1.208734f }, { 0.0f, 0.0f, 0.0f }, },
    // Stonehearth
    { { -162.115128f, -458.521393f, 40.389114f, 5.921130f }, { -146.283035f, -451.985413f, 40.392101f, 1.593585f },
    { -154.911880f, -440.288513f, 33.280674f, 3.392147f }, { -166.347992f, -458.094086f, 33.279636f, 4.809789f },
    { -146.530441f, -458.876343f, 26.394993f, 5.257466f }, { -146.420120f, -445.917725f, 26.408756f, 1.020243f },
    { -139.567215f, -456.592163f, 26.380287f, 2.649942f }, { 0.0f, 0.0f, 0.0f }, },
    // Iceblood
    { { -569.760010f, -263.288513f, 75.008682f, 2.104091f }, { -570.742493f, -271.907837f, 74.988449f, 3.238992f },
    { -578.531372f, -253.600327f, 74.959206f, 0.647177f }, { -561.223206f, -260.727386f, 68.457199f, 3.820190f },
    { -579.971008f, -265.969604f, 68.469582f, 2.068751f }, { -571.457458f, -256.716736f, 63.293938f, 0.509736f },
    { -567.616211f, -265.129028f, 59.324265f, 4.538830f }, { -579.480103f, -261.568329f, 52.497894f, 1.821352f },
    { -566.202026f, -260.122009f, 52.728100f, 5.634461f }, { 0.0f, 0.0f, 0.0f }, },
    // Tower Point
    { { -769.523010f, -358.549561f, 68.635841f, 3.853180f }, { -762.504395f, -362.904480f, 68.543678f, 5.730278f },
    { -761.258606f, -360.407471f, 72.666672f, 1.618719f }, { -772.757813f, -365.472168f, 79.463570f, 5.408264f },
    { -764.915161f, -358.086975f, 84.355766f, 3.004945f }, { -773.573181f, -366.593475f, 84.355766f, 4.414734f },
    { -777.062927f, -370.895477f, 90.868019f, 5.832375f }, { -758.860840f, -358.962280f, 90.825951f, 4.073083f },
    { -768.840515f, -362.985535f, 90.894974f, 4.304777f }, { 0.0f, 0.0f, 0.0f }, },
    // East Frostwolf
    { { -1304.755249f, -322.213409f, 91.419922f, 5.141234f }, { -1304.633057f, -310.950684f, 91.677505f, 0.746931f },
    { -1301.672974f, -309.800598f, 95.7477953f, 3.586145f }, { -1303.074585f, -321.931915f, 102.658630f, 0.259982f },
    { -1303.221680f, -310.574463f, 107.328194f, 3.590071f }, { -1302.911133f, -323.911835f, 107.328049f, 4.913466f },
    { -1302.600464f, -328.408600f, 113.846321f, 1.210312f }, { -1303.177612f, -307.762390f, 113.797401f, 6.177956f },
    { -1305.213989f, -315.778412f, 113.867081f, 5.251186f }, { 0.0f, 0.0f, 0.0f }, },
    // West Frostwolf
    { { -1299.614014f, -263.684998f, 114.151276f, 4.944896f }, { -1297.918335f, -269.959930f, 114.151276f, 4.772108f },
    { -1304.088379f, -272.878387f, 114.098404f, 5.200147f }, { -1287.346313f, -267.391785f, 114.115089f, 1.826860f },
    { -1298.288818f, -272.091309f, 107.612137f, 2.883221f }, { -1298.199463f, -258.249390f, 107.612183f, 3.052081f },
    { -1300.405640f, -273.112244f, 100.317612f, 4.929180f }, { -1291.555908f, -261.006134f, 91.648979f, 3.244500f },
    { -1299.686279f, -273.323853f, 91.8978200f, 3.876745f }, { 0.0f, 0.0f, 0.0f }, },
};

//////////////////////////////////////////////////////////////////////////
// Waypoints
//////////////////////////////////////////////////////////////////////////

// BIG TODO

//(06:46:20) (~compboy) FoS -> SH Bunker -> SH Outpost -> IW Bunker -> Dun Baldar

/*
lokholar ice lord spawn:
x: -301.823792
y: -271.596832
z: 6.667519
o: 3.5790528

ivus the forest lord
x: -221.667648
y: -311.846893
z: 6.667575
o: 2.295235
*/

static uint32 g_unitWaypoints[4][2] = { {  }, };

//////////////////////////////////////////////////////////////////////////
// Initial Guard Locations
//////////////////////////////////////////////////////////////////////////
const AVSpawnLocation g_initalGuardLocations[AV_NUM_CONTROL_POINTS][30] =
{
    { { 0.0f, 0.0f, 0.0f }, },  // Aid Station
    { { 0.0f, 0.0f, 0.0f }, },  // Stormpike Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Stonehearth Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Snowfall Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Coldtooth Mine
    { { 0.0f, 0.0f, 0.0f }, },  // Irondeep Mine
    { { 0.0f, 0.0f, 0.0f }, },  // Iceblood Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Frostwolf Graveyard
    { { 0.0f, 0.0f, 0.0f }, },  // Frostwolf Relief
    // Dun Baldar North
    { { 672.141296f, -120.239807f, 64.147659f, 2.379753f }, { 664.585083f, -126.076088f, 64.120972f, 2.768524f },
    { 661.464905f, -137.339371f, 64.216965f, 3.436112f }, { 0.0f, 0.0f, 0.0f }, },
    // Dun Baldar South
    { { 564.124146f, -71.107430f, 52.488060f, 1.300614f }, { 571.863770f, -77.084518f, 52.367657f, 0.295303f },
    { 574.969543f, -90.801270f, 52.412132f, 5.910901f }, { 0.0f, 0.0f, 0.0f }, },
    // Icewing
    { { 222.674530f, -378.605408f, 57.147060f, 5.577111f }, { 210.093506f, -384.971558f, 56.793076f, 4.595366f },
    { 199.602188f, -380.995575f, 56.864891f, 3.754991f }, { 192.782074f, -370.546204f, 57.015110f, 3.586132f },
    { 193.377426f, -360.313507f, 57.044708f, 2.891055f }, { 201.253113f, -351.977631f, 56.802952f, 3.036354f }, { 0.0f, 0.0f, 0.0f }, },
    // Stonehearth
    { { -139.692780f, -445.390533f, 40.982857f, 0.527005f }, { -140.723969f, -457.597168f, 40.862610f, 5.643875f },
    { -148.143784f, -464.959808f, 40.933720f, 5.639948f }, { -161.013535f, -466.793640f, 41.118568f, 4.512904f },
    { -170.595215f, -457.926331f, 40.875153f, 3.385859f }, { -172.157013f, -447.972717f, 40.888519f, 2.894984f }, { 0.0f, 0.0f, 0.0f }, },
    // Iceblood
    { { -568.816711f, -255.542038f, 75.008690f, 1.025736f }, { -580.106445f, -264.803497f, 74.931145f, 3.931711f },
    { -572.966248f, -271.499786f, 74.933746f, 4.198746f }, { -565.585876f, -268.645294f, 74.914063f, 5.459307f }, { 0.0f, 0.0f, 0.0f }, },
    // Tower Point
    { { -760.347595f, -358.281586f, 90.885597f, 0.358147f }, { -759.719116f, -367.059113f, 90.826775f, 5.722414f },
    { -768.441956f, -372.756653f, 90.933365f, 4.991992f }, { -775.595032f, -365.525177f, 90.894867f, 2.989226f },
    { -771.311890f, -353.218842f, 90.821220f, 1.795420f }, { 0.0f, 0.0f, 0.0f }, },
    // East Frostwolf
    { { -1297.069092f, -309.338623f, 113.769043f, 1.002180f }, { -1293.462036f, -316.529602f, 113.774048f, 0.067557f },
    { -1298.062256f, -326.274994f, 113.820679f, 5.015566f }, { -1311.626953f, -317.071228f, 113.775551f, 2.977457f },
    { -1305.169922f, -309.871796f, 113.824280f, 5.192283f }, { -1293.822021f, -317.798065f, 113.771339f, 3.103136f }, { 0.0f, 0.0f, 0.0f }, },
    // West Frostwolf
    { { -1299.964233f, -275.591461f, 114.055862f, 1.241742f }, { -1306.900757f, -268.969574f, 114.055481f, 3.464420f },
    { -1305.109375f, -261.103363f, 114.068550f, 2.298103f }, { -1295.956177f, -258.076904f, 114.056999f, 1.277085f },
    { -1288.039917f, -264.262085f, 114.115341f, 0.224653f }, { 0.0f, 0.0f, 0.0f }, },
};

//////////////////////////////////////////////////////////////////////////
// Home NPCs (that sit in the base)
//////////////////////////////////////////////////////////////////////////
const AVHomeNPC g_HomeNpcInfo[AV_NUM_CONTROL_POINTS] =
{
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // stormpike aid station
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // stormpike graveyard
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // stonehearth graveyard
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // snowfall graveyard
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // coldtooth mine
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // irondeep mine
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // iceblood graveyard
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // frostwolf graveyard
    { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },                                                                       // frostwolf relief hut
    { 14762, 14770, 726.534058f, -31.043175f, 50.621307f, 1.958136f,    -1373.700684f, -238.947479f, 99.362076f, 1.217441f },       // north dun baldar bunker
    { 14763, 14771, 722.385925f, -32.735390f, 50.621307f, 1.958136f,    -239.058929f, 99.367973f, 1.866966f },                      // south dun baldar bunker
    { 14764, 14774, 717.237793f, -34.835552f, 50.621307f, 1.958136f,    -1356.338135f, -232.723816f, 99.365128f, 2.250240f },       // icewing bunker
    { 14765, 14775, 711.430664f, -36.070595f, 50.621307f, 1.958136f,    -1349.830688f, -222.138000f, 99.370834f, 3.518692f },       // stonehearth bunker
    { 14766, 14773, 697.683472f, 3.335777f, 50.621426f, 5.021224f,      -1359.854248f, -212.682831f, 99.371040f, 4.256965f },       // iceblood tower
    { 14767, 14776, 700.351013f, 4.186835f, 50.621426f, 5.021224f,      -1372.945435f, -210.555832f, 99.371292f, 5.054141f },       // tower point
    { 14768, 14772, 703.704468f, 5.256737f, 50.621426f, 5.021224f,      -1383.414795f, -219.701355f, 99.371292f, 5.971476f },       // east frostwolf tower
    { 14769, 14777, 708.544128f, 6.800801f, 50.621426f, 5.021224f,      -1384.437012f, -233.046158f, 99.371338f, 0.227871f },       // west frostwolf tower
};

//////////////////////////////////////////////////////////////////////////
// Control Point Templates
//////////////////////////////////////////////////////////////////////////
static AVNodeTemplate g_nodeTemplates[AV_NUM_CONTROL_POINTS] =
{
    //      Name                GY?  Cap?           Graveyard Location
    { "Stormpike Aid Station", true, true, { 634.607971f, 47.408627f, 69.890488f },
    // Control Point Definition, ids, position vector, rotations
    // NEUTRAL  A-ASS   A-CON   H-ASS   H-CON                           POSITION                            ROTATION
    { { 179286, 179286, 179284, 179287, 179285 }, 639.184326f, -31.640333f, 46.202557f, 1.3546723f, 0.9025853f, -0.4305111f },
    // Aura Definition
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    // Glow Definition
    { { 180102, 180102, 180100, 180102, 180101 }, 639.184326f, -31.640333f, 46.202557f, 1.3546723f, 0.0f, 0.0f },
    // Guard IDs
    //N,   A,     H   guardC  boss ids peon locs   boss location  fire locations
    { 0, 12050, 12053 }, 5, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, 0,
    // World State Fields
    WORLDSTATE_AV_STORMPIKE_AID_STATION_ALLIANCE_ASSAULTING, WORLDSTATE_AV_STORMPIKE_AID_STATION_ALLIANCE_CONTROLLED,
    WORLDSTATE_AV_STORMPIKE_AID_STATION_HORDE_ASSAULTING, WORLDSTATE_AV_STORMPIKE_AID_STATION_HORDE_CONTROLLED,
    AV_NODE_STATE_ALLIANCE_CONTROLLED, }, // Default World State

    { "Stormpike Graveyard", true, true, { 675.970947f, -373.707336f, 29.780785f },
    { { 179286, 179286, 179284, 179287, 179285 }, 670.904175f, -294.181641f, 30.283506f, 4.232887f, 0.4067366f, 0.9135454f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 180102, 180102, 180100, 180102, 180101 }, 670.904175f, -294.181641f, 30.283506f, 4.232887f, 0.0f, 0.0f },
    { 0, 12050, 12053 }, 5, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, 0,
    WORLDSTATE_AV_STORMPIKE_GRAVE_ALLIANCE_ASSAULTING, WORLDSTATE_AV_STORMPIKE_GRAVE_ALLIANCE_CONTROLLED,
    WORLDSTATE_AV_STORMPIKE_GRAVE_HORDE_ASSAULTING, WORLDSTATE_AV_STORMPIKE_GRAVE_HORDE_CONTROLLED, AV_NODE_STATE_ALLIANCE_CONTROLLED, },

    { "Stonehearth Graveyard", true, true, { 73.869370f, -495.597656f, 48.740143f },
    { { 179286, 179286, 179284, 179287, 179285 }, 78.367012f, -404.769928f, 47.051014f, 4.350670f, 0.2672384f, 0.9636304f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 180102, 180102, 180100, 180102, 180101 }, 78.367012f, -404.769928f, 47.051014f, 4.350670f, 0.0f, 0.0f },
    { 0, 12050, 12053 }, 5, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, 0, WORLDSTATE_AV_STONEHEARTH_GRAVE_ALLIANCE_ASSAULTING,
    WORLDSTATE_AV_STONEHEARTH_GRAVE_ALLIANCE_CONTROLLED, WORLDSTATE_AV_STONEHEARTH_GRAVE_HORDE_ASSAULTING,
    WORLDSTATE_AV_STONEHEARTH_GRAVE_HORDE_CONTROLLED, AV_NODE_STATE_ALLIANCE_CONTROLLED, },

    { "Snowfall Graveyard", true, true, { -161.472916f, 34.797512f, 77.191841f },
    { { 180418, 179286, 179284, 179287, 179285 }, -202.571854f, -112.612862f, 78.489014f, 1.666146f, 0.9366722f, 0.3502073f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 180102, 180102, 180100, 180102, 180101 }, -202.571854f, -112.612862f, 78.489014f, 1.666146f, 0.0f, 0.0f },
    { 0, 12050, 12053 }, 5, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, WORLDSTATE_AV_SNOWFALL_GRAVE_NEUTRAL_CONTROLLED,
    WORLDSTATE_AV_SNOWFALL_GRAVE_ALLIANCE_ASSAULTING, WORLDSTATE_AV_SNOWFALL_GRAVE_ALLIANCE_CONTROLLED,
    WORLDSTATE_AV_SNOWFALL_GRAVE_HORDE_ASSAULTING, WORLDSTATE_AV_SNOWFALL_GRAVE_HORDE_CONTROLLED, AV_NODE_STATE_NEUTRAL_CONTROLLED, },

    { "Coldtooth Mine", false, true, { 0.0f, 0.0f, 0.0f }, { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, WORLDSTATE_AV_COLDTOOTH_MINE_KOBOLD_CONTROLLED, 0,
    WORLDSTATE_AV_COLDTOOTH_MINE_ALLIANCE_CONTROLLED, 0, WORLDSTATE_AV_COLDTOOTH_MINE_HORDE_CONTROLLED, AV_NODE_STATE_NEUTRAL_CONTROLLED, },

    { "Irondeep Mine", false, true, { 0.0f, 0.0f, 0.0f }, { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, WORLDSTATE_AV_IRONDEEP_MINE_TROGG_CONTROLLED, 0,
    WORLDSTATE_AV_IRONDEEP_MINE_ALLIANCE_CONTROLLED, 0, WORLDSTATE_AV_IRONDEEP_MINE_HORDE_CONTROLLED, AV_NODE_STATE_NEUTRAL_CONTROLLED, },

    { "Iceblood Graveyard", true, true, { -531.067627f, -405.459778f, 49.552971f },
    { { 179286, 179286, 179284, 179287, 179285 }, -611.891113f, -395.925751f, 60.798248f, 3.193738f, 0.9135455f, -0.4067366f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 180102, 180102, 180100, 180102, 180101 }, -611.891113f, -395.925751f, 60.798248f, 3.193738f, 0.0f, 0.0f },
    { 0, 12050, 12053 }, 5, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, 0, WORLDSTATE_AV_ICEBLOOD_GRAVE_ALLIANCE_ASSAULTING,
    WORLDSTATE_AV_ICEBLOOD_GRAVE_ALLIANCE_CONTROLLED, WORLDSTATE_AV_ICEBLOOD_GRAVE_HORDE_ASSAULTING,
    WORLDSTATE_AV_ICEBLOOD_GRAVE_HORDE_CONTROLLED, AV_NODE_STATE_HORDE_CONTROLLED, },

    { "Frostwolf Graveyard", true, true, { -1088.008545f, -248.774918f, 57.680843f },
    { { 179286, 179286, 179284, 179287, 179285 }, -1082.267578f, -344.454590f, 55.272388f, 1.579743f, 0.9025853f, -0.4305111f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 180102, 180102, 180100, 180102, 180101 }, -1082.267578f, -344.454590f, 55.272388f, 1.579743f, 0.0f, 0.0f },
    { 0, 12050, 12053 }, 5, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, 0, WORLDSTATE_AV_FROSTWOLF_GRAVE_ALLIANCE_ASSAULTING,
    WORLDSTATE_AV_FROSTWOLF_GRAVE_ALLIANCE_CONTROLLED, WORLDSTATE_AV_FROSTWOLF_GRAVE_HORDE_ASSAULTING,
    WORLDSTATE_AV_FROSTWOLF_GRAVE_HORDE_CONTROLLED, AV_NODE_STATE_HORDE_CONTROLLED, },

    { "Frostwolf Relief Hut", true, true, { -1500.063599f, -333.146393f, 101.133186f },
    { { 179286, 179286, 179284, 179287, 179285 }, -1401.263550f, -309.601624f, 89.412560f, 3.538534f, 0.4067366f, 0.9135454f },
    { { 0, 0, 0, 0, 0 }, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { { 180102, 180102, 180100, 180102, 180101 }, -1401.263550f, -309.601624f, 89.412560f, 3.538534f, 0.0f, 0.0f },
    { 0, 12050, 12053 }, 5, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 0, 0, WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_ALLIANCE_ASSAULTING,
    WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_ALLIANCE_CONTROLLED, WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_HORDE_ASSAULTING,
    WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_HORDE_CONTROLLED, AV_NODE_STATE_HORDE_CONTROLLED, },

    { "North Dun Baldar Bunker", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 179284, 179287, 0 }, 673.361877f, -144.169357f, 63.651852f, 0.969958f, 0.2672384f, 0.9636304f },
    { { 0, 179446, 178927, 179446, 0 }, 678.393250f, -136.145126f, 76.004265f, 4.515254f, 0.0f, 0.0f },
    { { 0, 180423, 180421, 180423, 0 }, 678.393250f, -136.145126f, 76.004265f, 4.515254f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13358, 0, 0, WORLDSTATE_AV_DUN_BALDAR_NORTH_BUNKER_CONTROLLED,
    WORLDSTATE_AV_DUN_BALDAR_NORTH_BUNKER_ASSAULTING, WORLDSTATE_AV_DUN_BALDAR_NORTH_BUNKER_DESTROYED, AV_NODE_STATE_ALLIANCE_CONTROLLED, },

    { "South Dun Baldar Bunker", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 179284, 179287, 0 }, 552.990234f, -77.527374f, 51.926868f, 3.5410520f, 0.9366722f, 0.3502073f },
    { { 0, 179450, 178932, 179450, 0 }, 556.081543f, -84.437355f, 64.411903f, 5.006125f, 0.0f, 0.0f },
    { { 0, 180423, 180421, 180423, 0 }, 556.081543f, -84.437355f, 64.411903f, 5.006125f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13358, 0, 0, WORLDSTATE_AV_DUN_BALDAR_SOUTH_BUNKER_CONTROLLED,
    WORLDSTATE_AV_DUN_BALDAR_SOUTH_BUNKER_ASSAULTING, WORLDSTATE_AV_DUN_BALDAR_SOUTH_BUNKER_DESTROYED, AV_NODE_STATE_ALLIANCE_CONTROLLED, },

    { "Icewing Bunker", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 179284, 179287, 0 }, 201.845840f, -358.886963f, 56.370701f, 5.403111f, 0.9135455f, -0.4067366f },
    { { 0, 179454, 178947, 179454, 0 }, 208.691223f, -367.553314f, 68.696632f, 3.317518f, 0.0f, 0.0f },
    { { 0, 180423, 180421, 180423, 0 }, 208.691223f, -367.553314f, 68.696632f, 3.317518f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13358, 0, 0, WORLDSTATE_AV_ICEWING_BUNKER_CONTROLLED,
    WORLDSTATE_AV_ICEWING_BUNKER_ASSAULTING, WORLDSTATE_AV_ICEWING_BUNKER_DESTROYED, AV_NODE_STATE_ALLIANCE_CONTROLLED, },

    { "Stonehearth Bunker", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 179284, 179287, 0 }, -151.776291f, -439.776245f, 40.381840f, 4.319230f, 0.9025853f, -0.4305111f },
    { { 0, 179458, 178948, 179458, 0 }, -156.257721f, -448.993652f, 52.797112f, 2.803080f, 0.0f, 0.0f },
    { { 0, 180423, 180421, 180423, 0 }, -156.257721f, -448.993652f, 52.797112f, 2.803080f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13358, 0, 0, WORLDSTATE_AV_STONEHEARTH_BUNKER_CONTROLLED,
    WORLDSTATE_AV_STONEHEARTH_BUNKER_ASSAULTING, WORLDSTATE_AV_STONEHEARTH_BUNKER_DESTROYED, AV_NODE_STATE_ALLIANCE_CONTROLLED, },

    { "Iceblood Tower", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 0, 179287, 179285 }, -572.502869f, -262.418365f, 75.008713f, 5.483192f, 0.4067366f, 0.9135454f },
    { { 0, 179436, 0, 179436, 178955 }, -572.408386f, -262.620422f, 88.636307f, 0.712354f, 0.0f, 0.0f },
    { { 0, 180423, 0, 180423, 180422 }, -572.408386f, -262.620422f, 88.636307f, 0.712354f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13359, 0, WORLDSTATE_AV_ICEBLOOD_TOWER_ASSAULTING,
    WORLDSTATE_AV_ICEBLOOD_TOWER_DESTROYED, 0, WORLDSTATE_AV_ICEBLOOD_TOWER_CONTROLLED, AV_NODE_STATE_HORDE_CONTROLLED, },

    { "Tower Point", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 0, 179287, 179285 }, -767.986511f, -363.091064f, 90.894920f, 4.265791f, 0.2672384f, 0.9636304f },
    { { 0, 179440, 0, 179440, 178956 }, -768.181519f, -362.942719f, 104.553452f, 4.761085f, 0.0f, 0.0f },
    { { 0, 180423, 0, 180423, 180422 }, -768.181519f, -362.942719f, 104.553452f, 4.761085f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13359, 0, WORLDSTATE_AV_TOWERPOINT_ASSAULTING,
    WORLDSTATE_AV_TOWERPOINT_DESTROYED, 0, WORLDSTATE_AV_TOWERPOINT_CONTROLLED, AV_NODE_STATE_HORDE_CONTROLLED, },

    { "East Frostwolf Tower", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 0, 179287, 179285 }, -1302.814453f, -315.957458f, 113.867195f, 1.956738f, 0.9366722f, 0.3502073f },
    { { 0, 179442, 0, 179442, 178957 }, -1302.916626f, -316.606079f, 127.526337f, 6.176374f, 0.0f, 0.0f },
    { { 0, 180423, 0, 180423, 180422 }, -1302.916626f, -316.606079f, 127.526337f, 6.176374f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13359, 0, WORLDSTATE_AV_FROSTWOLF_EAST_TOWER_ASSAULTING,
    WORLDSTATE_AV_FROSTWOLF_EAST_TOWER_DESTROYED, 0, WORLDSTATE_AV_FROSTWOLF_EAST_TOWER_CONTROLLED, AV_NODE_STATE_HORDE_CONTROLLED, },

    { "West Frostwolf Tower", false, false, { 0.0f, 0.0f, 0.0f },
    { { 0, 179286, 0, 179287, 179285 }, -1296.913818f, -266.521271f, 114.151245f, 3.336680f, 0.9135455f, -0.4067366f },
    { { 0, 179444, 0, 179444, 178958 }, -1297.889404f, -266.714233f, 127.789467f, 5.931325f, 0.0f, 0.0f },
    { { 0, 180423, 0, 180423, 180422 }, -1297.889404f, -266.714233f, 127.789467f, 5.931325f, 0.0f, 0.0f },
    { 0, 0, 0 }, 0, { 0, 0, 0 }, NULL, { 0.0f, 0.0f, 0.0f }, 13359, 0, WORLDSTATE_AV_FROSTWOLF_WEST_TOWER_ASSAULTING,
    WORLDSTATE_AV_FROSTWOLF_WEST_TOWER_DESTROYED, 0, WORLDSTATE_AV_FROSTWOLF_WEST_TOWER_CONTROLLED, AV_NODE_STATE_HORDE_CONTROLLED, },
};

// gameobject faction def'n
static uint32 g_gameObjectFactions[AV_NODE_STATE_COUNT] = { 35, 2, 2, 1, 1, };
const static int32 g_stateToGuardType[AV_NODE_STATE_COUNT] = { 0, -1, 1, -2, 2, };
static const char *g_stateNames[AV_NODE_STATE_COUNT] = { "AV_NODE_STATE_NEUTRAL_CONTROLLED", "AV_NODE_STATE_ALLIANCE_ASSAULTING",
"AV_NODE_STATE_ALLIANCE_CONTROLLED", "AV_NODE_STATE_HORDE_ASSAULTING", "AV_NODE_STATE_HORDE_CONTROLLED", };

AVNode::AVNode( AlteracValley* parent, AVNodeTemplate *tmpl, uint32 nodeid) : m_template(tmpl), m_nodeId(nodeid)
{
    m_bg = parent;
    m_boss = NULL;
    m_flag = NULL;
    m_aura = NULL;
    m_glow = NULL;
    m_state = tmpl->m_defaultState;
    m_lastState = tmpl->m_defaultState;
    m_destroyed = false;
    m_homeNPC = NULL;
    m_spiritGuide = NULL;

    // set state
    ChangeState(m_state);

    // spawn initial guards (if we have any)
    if( !m_template->m_capturable && m_template->m_initialSpawnId )
    {
        // then we are probably a tower.
        const AVSpawnLocation *spi = g_initalGuardLocations[nodeid];
        CreatureData *ctrData = sCreatureDataMgr.GetCreatureData(m_template->m_initialSpawnId);
        sLog.outDebug("spawning guards at bunker %s of %s (%u)", m_template->m_name, ctrData->Name, ctrData->Entry);

        while(spi->x != 0.0f)
        {
            if(Creature *sp = m_bg->GetMapMgr()->CreateCreature(ctrData->Entry))
            {
                sp->Load(MODE_5PLAYER_NORMAL, spi->x, spi->y, spi->z, spi->o);
                sp->PushToWorld(m_bg->GetMapMgr());
            }
            ++spi;
        }
    }

    // spawn initial home npc
    if( g_HomeNpcInfo[m_nodeId].id_a != 0 )
    {
        // spawn alliance npcs if its a horde tower
        if( m_template->m_defaultState == AV_NODE_STATE_ALLIANCE_CONTROLLED )
        {
            m_homeNPC = m_bg->SpawnCreature(g_HomeNpcInfo[m_nodeId].id_a, g_HomeNpcInfo[m_nodeId].a_x, g_HomeNpcInfo[m_nodeId].a_y,
                g_HomeNpcInfo[m_nodeId].a_z, g_HomeNpcInfo[m_nodeId].a_o);
        }
        else
        {
            m_homeNPC = m_bg->SpawnCreature(g_HomeNpcInfo[m_nodeId].id_h, g_HomeNpcInfo[m_nodeId].h_x, g_HomeNpcInfo[m_nodeId].h_y,
                g_HomeNpcInfo[m_nodeId].h_z, g_HomeNpcInfo[m_nodeId].h_o);
        }
    }
}

AVNode::~AVNode()
{

}

void AVNode::Assault(Player* plr)
{
    // player assaulted the control point.
    // safety check
    sLog.outDebug("AVNode::Assault(%s) : entering", m_template->m_name);
    if( plr->GetTeam() == 0 && ( m_state == AV_NODE_STATE_ALLIANCE_ASSAULTING || m_state == AV_NODE_STATE_ALLIANCE_CONTROLLED ) )
        return;

    if( plr->GetTeam() == 1 && ( m_state == AV_NODE_STATE_HORDE_ASSAULTING || m_state == AV_NODE_STATE_HORDE_CONTROLLED ) )
        return;

    if( m_destroyed )
        return;

    // are we returning the node to us?
    if( ( plr->GetTeam() == 0 && m_lastState == AV_NODE_STATE_ALLIANCE_CONTROLLED ) ||
        ( plr->GetTeam() == 1 && m_lastState == AV_NODE_STATE_HORDE_CONTROLLED ) )
    {
        // set the state for capture
        m_state = (plr->GetTeam() == 0) ? AV_NODE_STATE_ALLIANCE_ASSAULTING : AV_NODE_STATE_HORDE_ASSAULTING;

        // no timer delay
        Capture();

        // pvp data
        if( m_template->m_isGraveyard )
            plr->m_bgScore.MiscData[BG_SCORE_AV_GRAVEYARDS_DEFENDED]++;
        else
            plr->m_bgScore.MiscData[BG_SCORE_AV_TOWERS_DEFENDED]++;

        return;
    }

    // start timer blablabla
    sEventMgr.RemoveEvents(m_bg, EVENT_AV_CAPTURE_CP_0 + m_nodeId);
    sEventMgr.AddEvent(m_bg, &AlteracValley::EventAssaultControlPoint, m_nodeId, EVENT_AV_CAPTURE_CP_0 + m_nodeId, TIME_MINUTE * 4 * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

    // update state
    ChangeState(plr->GetTeam() ? AV_NODE_STATE_HORDE_ASSAULTING : AV_NODE_STATE_ALLIANCE_ASSAULTING);

    // pvp data
    if( m_template->m_isGraveyard )
    {
        // send message
        m_bg->SendChatMessage(CHAT_MSG_BG_SYSTEM_ALLIANCE + plr->GetTeam(), 0, "%s claims the %s! If left unchallenged, the %s will control it!", plr->GetName(), m_template->m_name,
            plr->GetTeam() ? "Horde" : "Alliance");

        plr->m_bgScore.MiscData[BG_SCORE_AV_GRAVEYARDS_ASSAULTED]++;
    }
    else
    {
        m_bg->Herald("%s is under attack! If left unchecked the %s will destroy it!", m_template->m_name , plr->GetTeam() ? "Horde" : "Alliance");
        plr->m_bgScore.MiscData[BG_SCORE_AV_TOWERS_ASSAULTED]++;
    }
}

void AVNode::Spawn()
{
    // spawn control point (if we should have one)
    sLog.outDebug("AVNode::Spawn for %s, state = %u %s, old_state = %u %s", m_template->m_name, m_state, g_stateNames[m_state], m_lastState, g_stateNames[m_lastState]);
    if( m_template->m_flagLocation.id[m_state] == 0 )
    {
        // no flag should be spawned, despawn if one exists
        if( m_flag != NULL )
        {
            sLog.outDebug("AVNode::Spawn(%s) : Despawning main flag", m_template->m_name);
            m_flag->Despawn(0, 0);
            m_flag = NULL;
        }
    }
    else
    {
        // spawn the flag.
        const AVGameObject* g = &m_template->m_flagLocation;
        sLog.outDebug("AVNode::Spawn(%s) : Spawning main flag", m_template->m_name);
        if(m_flag && m_flag->GetEntry() != g->id[m_state])
        {
            if(m_flag->IsInWorld())
                m_flag->RemoveFromWorld(true);
            else m_flag->Destruct();
            m_flag = NULL;
        }
        else if(m_flag && !m_flag->IsInWorld())
            m_flag->PushToWorld(m_bg->GetMapMgr());

        if( m_flag == NULL )
        {
            // initial spawn
            m_flag = m_bg->SpawnGameObject(g->id[m_state], g->x, g->y, g->z, g->o, 0, 0, 1.0f);
            m_flag->bannerslot = m_nodeId;
            m_flag->SetUInt32Value(GAMEOBJECT_FACTION, g_gameObjectFactions[m_state]);
            m_flag->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
            m_flag->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
            m_flag->PushToWorld(m_bg->GetMapMgr());
        }
    }

    // spawn secondary control node aura (if there should be one)
    if( m_template->m_auraLocation.id[m_state] == 0 )
    {
        // no flag should be spawned, despawn if one exists
        if( m_aura != NULL )
        {
            sLog.outDebug("AVNode::Spawn(%s) : Despawning secondary flag", m_template->m_name);
            m_aura->Despawn(0, 0);
            m_aura = NULL;
        }
    }
    else
    {
        // spawn the flag.
        const AVGameObject* g = &m_template->m_auraLocation;
        sLog.outDebug("AVNode::Spawn(%s) : Spawning secondary flag", m_template->m_name);
        if(m_aura != NULL)
        {
            // change entry, but to do this change guid
            if( m_aura->GetEntry() != g->id[m_state] )
            {
                if(m_aura->IsInWorld())
                    m_aura->RemoveFromWorld(true);
                else m_aura->Destruct();
                m_aura = NULL;
            }
            else if(!m_aura->IsInWorld())
                m_aura->PushToWorld(m_bg->GetMapMgr());
        }

        if( m_aura == NULL )
        {
            // initial spawn
            m_aura = m_bg->SpawnGameObject(g->id[m_state], g->x, g->y, g->z, g->o, 0, 0, 3.0f);
            m_aura->SetUInt32Value(GAMEOBJECT_FACTION, g_gameObjectFactions[m_state]);
            m_aura->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
            m_aura->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
            m_aura->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
            m_aura->PushToWorld(m_bg->GetMapMgr());
        }
    }

    // spawn glow
    if( m_template->m_glowLocation.id[m_state] == 0 )
    {
        // no flag should be spawned, despawn if one exists
        if( m_glow != NULL )
        {
            sLog.outDebug("AVNode::Spawn(%s) : Despawning glow", m_template->m_name);
            m_glow->Despawn(0, 0);
            m_glow = NULL;
        }
    }
    else
    {
        // spawn the flag.
        const AVGameObject* g = &m_template->m_glowLocation;
        sLog.outDebug("AVNode::Spawn(%s) : Spawning glow", m_template->m_name);
        if(m_glow)
        {
            if(m_glow->GetEntry() == g->id[m_state])
                m_glow->AddToWorld();
            else
            {
                if(m_glow->IsInWorld())
                    m_glow->RemoveFromWorld(true);
                else m_glow->Destruct();
                m_glow = NULL;
            }
        }

        if( m_glow == NULL )
        {
            // initial spawn
            m_glow = m_bg->SpawnGameObject(g->id[m_state], g->x, g->y, g->z, g->o, 0, 0, 1.0f);
            m_glow->SetUInt32Value(GAMEOBJECT_FACTION, g_gameObjectFactions[m_state]);
            m_glow->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
            m_glow->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
            m_glow->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
            if(m_glow->GetEntry() == 180422 || m_glow->GetEntry() == 180423)
                m_glow->SetFloatValue(OBJECT_FIELD_SCALE_X, 10.0f);
            else
                m_glow->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
            m_glow->PushToWorld(m_bg->GetMapMgr());
        }
    }

    // update field states :O
    for(uint32 i = 0; i < AV_NUM_SPAWN_TYPES; i++)
    {
        if( m_template->m_worldStateFields[i] )
            m_bg->GetMapMgr()->GetStateManager().UpdateWorldState( m_template->m_worldStateFields[i], 0 );
    }

    if( m_template->m_worldStateFields[m_state] != 0 )
        m_bg->GetMapMgr()->GetStateManager().UpdateWorldState(m_template->m_worldStateFields[m_state], 1);

    // despawn/spawn guards
    if( m_state == AV_NODE_STATE_ALLIANCE_CONTROLLED || m_state == AV_NODE_STATE_HORDE_CONTROLLED )
    {
        sLog.outDebug("AVNode::Spawn(%s) : despawning guards", m_template->m_name);
        for(std::vector<Creature*>::iterator itr = m_guards.begin(); itr != m_guards.end(); itr++)
            (*itr)->Despawn(0, 0);

        m_guards.clear();

        // spawn guards if needed
        uint32 t = g_stateToGuardType[m_state];
        if( t > 0 && m_template->m_guardId[t] != 0 )
        {
            sLog.outDebug("AVNode::Spawn(%s) : spawning %u guards of %u", m_template->m_name, m_template->m_guardCount, m_template->m_guardId[t]);
            for(uint32 i = 0; i < m_template->m_guardCount; i++)
            {
                float x = RandomUInt(10) * cos(RandomFloat(6.28f)) + m_template->m_flagLocation.x;
                float y = RandomUInt(10) * cos(RandomFloat(6.28f)) + m_template->m_flagLocation.y;
                float z = m_bg->GetMapMgr()->GetLandHeight(x, y);
                if(z == NO_LAND_HEIGHT)
                    z = m_template->m_flagLocation.z;

                if( z != 0.0f )
                    m_guards.push_back(m_bg->SpawnCreature(m_template->m_guardId[t], x, y, z, 0.0f));
            }
        }
    }

    if( g_stateToGuardType[m_state] > 0 )
        SpawnGuards(g_stateToGuardType[m_state]);

    // spirit guides
    if( m_template->m_isGraveyard )
    {
        // should one be spawned
        if( m_spiritGuide != NULL )
        {
            sLog.outDebug("AVNode::Spawn(%s) : despawning spirit guide", m_template->m_name);
            // move everyone in the revive queue to a different node
            std::map<Creature*, std::set<WoWGuid> >::iterator itr = m_bg->m_resurrectMap.find(m_spiritGuide);
            if( itr != m_bg->m_resurrectMap.end() )
            {
                for(std::set<WoWGuid>::iterator it2 = itr->second.begin(); it2 != itr->second.end(); it2++)
                {
                    // repop him at a new GY
                    Player* plr_tmp = m_bg->GetMapMgr()->GetPlayer(*it2);
                    if( plr_tmp != NULL )
                    {
                        m_bg->HookHandleRepop(plr_tmp);
                        m_bg->QueueAtNearestSpiritGuide(plr_tmp, m_spiritGuide);
                    }
                }
                itr->second.clear();
            }
            m_bg->RemoveSpiritGuide(m_spiritGuide);
            m_spiritGuide->Despawn(0, 0);
            m_spiritGuide = NULL;
        }

        if( m_state == AV_NODE_STATE_ALLIANCE_CONTROLLED )
        {
            sLog.outDebug("AVNode::Spawn(%s) : spawning spirit guide", m_template->m_name);

            // spawn new spirit guide
            m_spiritGuide = m_bg->SpawnSpiritGuide(m_template->m_graveyardLocation.x, m_template->m_graveyardLocation.y,
                m_template->m_graveyardLocation.z, m_template->m_graveyardLocation.z, false);

            // add
            m_bg->AddSpiritGuide(m_spiritGuide);
        }
        else if( m_state == AV_NODE_STATE_HORDE_CONTROLLED )
        {
            sLog.outDebug("AVNode::Spawn(%s) : spawning spirit guide", m_template->m_name);

            // spawn new spirit guide
            m_spiritGuide = m_bg->SpawnSpiritGuide(m_template->m_graveyardLocation.x, m_template->m_graveyardLocation.y,
                m_template->m_graveyardLocation.z, m_template->m_graveyardLocation.z, true);

            // add
            m_bg->AddSpiritGuide(m_spiritGuide);
        }
    }
    sLog.outDebug("AVNode::Spawn(%s) : completed for state %u %s", m_template->m_name, m_state, g_stateNames[m_state]);
}

void AVNode::SpawnGuards(uint32 x)
{
    sLog.outDebug("AVNode::SpawnGuards(%s) : spawning %u guards", m_template->m_name, m_template->m_guardCount);
}

void AVNode::ChangeState(uint32 new_state)
{
    sLog.outDebug("AVNode::ChangeState(%s) : changing to state %u %s, old state was %u %s", m_template->m_name, new_state, g_stateNames[new_state], m_state, g_stateNames[m_state]);
    m_lastState = m_state;
    m_state = new_state;

    // update spawns
    Spawn();
}

void AVNode::Capture()
{
    if( m_destroyed )
        return;

    sLog.outDebug("AVNode::Capture(%s) : entering", m_template->m_name);

    // sooo easy
    sEventMgr.RemoveEvents(m_bg, EVENT_AV_CAPTURE_CP_0 + m_nodeId);
    if( m_state == AV_NODE_STATE_HORDE_ASSAULTING )
        ChangeState(AV_NODE_STATE_HORDE_CONTROLLED);
    else
        ChangeState(AV_NODE_STATE_ALLIANCE_CONTROLLED);

    // were we destroyed?
    if( !m_template->m_capturable )
    {
        if( m_state != m_template->m_defaultState )
        {
            // spawn fire!
            const AVSpawnLocation *spi = g_fireLocations[m_nodeId];
            GameObject* go;

            sLog.outDebug("spawning fires at bunker %s", m_template->m_name);
            while(spi->x != 0.0f)
            {
                go = m_bg->SpawnGameObject(AV_GAMEOBJECT_FIRE, spi->x, spi->y, spi->z, spi->o, 0, 35, 1.0f);
                go->PushToWorld(m_bg->GetMapMgr());
                ++spi;
            }

            // disable the flag
            if( m_flag != NULL )
            {
                m_flag->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                m_flag->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0);
                m_flag->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
            }

            // prevent further actions
            m_destroyed = true;

            // send message
            m_bg->Herald("The %s has destroyed the %s.", (m_template->m_defaultState == AV_NODE_STATE_ALLIANCE_CONTROLLED) ? "Horde" : "Alliance", m_template->m_name);

            if( m_template->m_defaultState == AV_NODE_STATE_ALLIANCE_CONTROLLED )
            {
                m_bg->GiveHonorToTeam(1, m_bg->GetBonusHonor() * 3);
            }
            else if( m_template->m_defaultState == AV_NODE_STATE_HORDE_CONTROLLED )
            {
                m_bg->GiveHonorToTeam(0, m_bg->GetBonusHonor() * 3);
            }

            // the npc at our home has to change.
            if( m_homeNPC != NULL )
                m_homeNPC->Despawn(0, 0);

            // respawn if we have one
            if( g_HomeNpcInfo[m_nodeId].id_a != 0 )
            {
                // spawn alliance npcs if its a horde tower
                if( m_template->m_defaultState == AV_NODE_STATE_HORDE_CONTROLLED )
                {
                    m_homeNPC = m_bg->SpawnCreature(g_HomeNpcInfo[m_nodeId].id_a, g_HomeNpcInfo[m_nodeId].a_x, g_HomeNpcInfo[m_nodeId].a_y,
                        g_HomeNpcInfo[m_nodeId].a_z, g_HomeNpcInfo[m_nodeId].a_o);
                }
                else
                {
                    m_homeNPC = m_bg->SpawnCreature(g_HomeNpcInfo[m_nodeId].id_h, g_HomeNpcInfo[m_nodeId].h_x, g_HomeNpcInfo[m_nodeId].h_y,
                        g_HomeNpcInfo[m_nodeId].h_z, g_HomeNpcInfo[m_nodeId].h_o);
                }
            }

            // lose 75 reinforcements
            if( m_template->m_defaultState == AV_NODE_STATE_ALLIANCE_CONTROLLED )
                m_bg->RemoveReinforcements(0, AV_POINTS_ON_DESTROY_BUNKER);
            else
                m_bg->RemoveReinforcements(1, AV_POINTS_ON_DESTROY_BUNKER);
        }
        else
        {
            // saved the tower
            m_bg->Herald("The %s has taken the %s.", (m_state == AV_NODE_STATE_ALLIANCE_CONTROLLED) ? "Alliance" : "Horde", m_template->m_name);
        }
    }
    else
    {
        // captured message
        m_bg->Herald("The %s has taken the %s.", (m_state == AV_NODE_STATE_ALLIANCE_CONTROLLED) ? "Alliance" : "Horde", m_template->m_name);
    }
}

AlteracValley::AlteracValley( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr,id,lgroup,t)
{
    m_playerCountPerTeam = 40;
    m_reinforcements[0] = AV_NUM_REINFORCEMENTS;
    m_reinforcements[1] = AV_NUM_REINFORCEMENTS;
    m_LiveCaptain[0] = true;
    m_LiveCaptain[1] = true;
    m_bonusHonor = HonorHandler::CalculateHonorPoints(lgroup*10,lgroup*10);

    memset(m_nodes, 0, sizeof(m_nodes));
}

void AlteracValley::Init()
{
    CBattleground::Init();
}

AlteracValley::~AlteracValley()
{

}

bool AlteracValley::HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell)
{
    // burlex todo: find a cleaner way to do this that doesnt waste memory.
    if(pGo->bannerslot >= 0 && pGo->bannerslot < AV_NUM_CONTROL_POINTS)
    {
        // TODO: anticheat here
        m_nodes[pGo->bannerslot]->Assault(pPlayer);
        CBattleground::HookSlowLockOpen(pGo, pPlayer, pSpell);
        return true;
    }

    CBattleground::HookSlowLockOpen(pGo, pPlayer, pSpell);
    return false;
}

void AlteracValley::HookFlagDrop(Player* plr, GameObject* obj)
{

}

void AlteracValley::HookFlagStand(Player* plr, GameObject* obj)
{

}

void AlteracValley::HookOnMount(Player* plr)
{

}

void AlteracValley::HookOnAreaTrigger(Player* plr, uint32 id)
{

}

bool AlteracValley::HookHandleRepop(Player* plr)
{
    uint32 x;
    float dist = 999999.0f;
    float dt;
    LocationVector dest_pos;
    if( plr->GetTeam() == 1 )
        dest_pos.ChangeCoords(-1433.550903f, -608.329529f, 51.149689f);
    else
        dest_pos.ChangeCoords(876.434448f, -489.599579f, 96.517174f);

    if(m_started)
    {
        for(x = 0; x < AV_NUM_CONTROL_POINTS; ++x)
        {
            // skip non-graveyards
            if( !m_nodes[x]->m_template->m_isGraveyard )
                continue;

            // make sure they're owned by us
            if( ( plr->GetTeam() == 0 && m_nodes[x]->m_state == AV_NODE_STATE_ALLIANCE_CONTROLLED ) ||
                ( plr->GetTeam() == 1 && m_nodes[x]->m_state == AV_NODE_STATE_HORDE_CONTROLLED ) )
            {
                dt = plr->GetPositionNC().Distance2DSq(m_nodes[x]->m_template->m_graveyardLocation.x, m_nodes[x]->m_template->m_graveyardLocation.y);
                if( dt < dist )
                {
                    // new one
                    dest_pos.ChangeCoords(m_nodes[x]->m_template->m_graveyardLocation.x, m_nodes[x]->m_template->m_graveyardLocation.y, m_nodes[x]->m_template->m_graveyardLocation.z);
                    dist = dt;
                }
            }
        }
    }

    // port to it
    plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest_pos);
    return false;
}

void AlteracValley::HookOnHK(Player* plr)
{
    plr->m_bgScore.HonorableKills++;
    UpdatePvPData();
}

void AlteracValley::DropFlag(Player* plr)
{

}

void AlteracValley::OnCreate()
{
    WorldStateManager &sm = m_mapMgr->GetStateManager();
    sm.CreateWorldState(WORLDSTATE_AV_ALLIANCE_SCORE_DISPLAY, 1);
    sm.CreateWorldState(WORLDSTATE_AV_HORDE_SCORE_DISPLAY, 1);

    sm.CreateWorldState(WORLDSTATE_AV_ALLIANCE_SCORE, 600);
    sm.CreateWorldState(WORLDSTATE_AV_HORDE_SCORE, 600);

    // Alliance Gate
    GameObject* gate = SpawnGameObject(AV_GAMEOBJECT_GATE, 780.487f, -493.024f, 99.9553f, 3.0976f, 32, 114, 3.000000f);
    gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
    gate->PushToWorld(m_mapMgr);
    m_gates.push_back(gate);

    // Horde gate
    gate = SpawnGameObject(AV_GAMEOBJECT_GATE, -1375.73f, -538.966f, 55.3006f, 0.791198f, 32, 114, 3.000000f);
    gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
    gate->PushToWorld(m_mapMgr);
    m_gates.push_back(gate);

    for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; i++)
    {
        for(uint32 j = 0; j < AV_NODE_STATE_COUNT; ++j)
        {
            if( g_nodeTemplates[i].m_worldStateFields[j] != 0 )
                sm.CreateWorldState(g_nodeTemplates[i].m_worldStateFields[j], 0);
        }
    }

    for(uint32 x = 0; x < AV_NUM_CONTROL_POINTS; ++x)
        m_nodes[x] = new AVNode(this, &g_nodeTemplates[x], x);

    // generals/leaders!
    SpawnCreature(AV_NPC_GENERAL_VANNDAR_STORMPIKE, 726.969604f, -9.716300f, 50.621391f, 3.377580f);
    SpawnCreature(AV_NPC_GENERAL_DREK_THAR, -1367.080933f, -229.453140f, 98.421570f, 2.023553f);

    // some captains
    SpawnCreature(AV_NPC_CAPTAIN_BALINDA_STONEHEARTH, -57.368469f, -286.770966f, 15.564562f, 6.068771f);
    SpawnCreature(AV_NPC_CAPTAIN_GALVANGAR, -537.177429f, -168.004944f, 57.008938f, 2.749793f);

    // home spirit guides
    AddSpiritGuide(SpawnSpiritGuide(876.434448f, -489.599579f, 96.517174f, 0.0f, false));
    AddSpiritGuide(SpawnSpiritGuide(-1433.550903f, -608.329529f, 51.149689f, 0.0f, true));
}

void AlteracValley::OnStart()
{
    for(uint32 i = 0; i < 2; i++)
    {
        for(std::set<Player*  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); itr++)
        {
            (*itr)->RemoveAura(BG_PREPARATION);
        }
    }

    // open gates
    for(std::list< GameObject* >::iterator itr = m_gates.begin(); itr != m_gates.end(); itr++)
    {
        (*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, 64);
        (*itr)->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 0);
    }

    /* correct? - burlex */
    PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

    m_started = true;

    sEventMgr.AddEvent(this, &AlteracValley::EventUpdateResources, EVENT_BATTLEGROUND_RESOURCEUPDATE, AV_REINFORCEMENT_ADD_INTERVAL, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void AlteracValley::OnAddPlayer(Player* plr)
{
    if(!m_started)
        plr->CastSpell(plr, BG_PREPARATION, true);

    if( plr->GetTeam() )
    {
        plr->SetAtWar( 730, true );
        plr->SetStanding( 730, -9000);
    }
    else
    {
        plr->SetAtWar( 729, true );
        plr->SetStanding( 729, -9000);
    }
}

void AlteracValley::OnRemovePlayer(Player* plr)
{
    plr->RemoveAura(BG_PREPARATION);
}

LocationVector AlteracValley::GetStartingCoords(uint32 Team)
{
    if(Team)
        return LocationVector(-1433.550903f, -608.329529f, 51.149689f, 1.140702f);
    else
        return LocationVector(876.434448f, -489.599579f, 96.517174f, 3.222486f);
}

void AlteracValley::HookOnPlayerDeath(Player* plr)
{
    RemoveReinforcements(plr->GetTeam(), AV_POINTS_ON_KILL);
}

void AlteracValley::AddReinforcements(uint32 teamId, uint32 amt)
{
    WorldStateManager &sm = m_mapMgr->GetStateManager();
    if( ((int32)( m_reinforcements[teamId] + amt )) > AV_NUM_REINFORCEMENTS )
        m_reinforcements[teamId] = AV_NUM_REINFORCEMENTS;
    else
        m_reinforcements[teamId] += amt;

    sm.UpdateWorldState(WORLDSTATE_AV_ALLIANCE_SCORE + teamId, m_reinforcements[teamId]);
}

void AlteracValley::RemoveReinforcements(uint32 teamId, uint32 amt)
{
    WorldStateManager &sm = m_mapMgr->GetStateManager();
    if( ((int32)( m_reinforcements[teamId] - amt )) < 0 )
        m_reinforcements[teamId] = 0;
    else
        m_reinforcements[teamId] -= amt;

    sm.UpdateWorldState(WORLDSTATE_AV_ALLIANCE_SCORE + teamId, m_reinforcements[teamId]);

    // We've lost. :(
    if( m_reinforcements[teamId] == 0 )
    {
        Finish( teamId );
    }
}

void AlteracValley::HookOnPlayerKill(Player* plr, Unit* pVictim)
{
    if(pVictim->IsPlayer())
    {
        plr->m_bgScore.KillingBlows++;
        UpdatePvPData();
    }
}

void AlteracValley::HookOnUnitKill(Player* plr, Unit* pVictim)
{
    if(pVictim->IsPlayer())
        return;

    if(pVictim->GetEntry() == AV_NPC_GENERAL_VANNDAR_STORMPIKE)
    {
        Herald("The Stormpike General is dead!");
        RemoveReinforcements( 0, AV_NUM_REINFORCEMENTS );
        GiveHonorToTeam(1, m_bonusHonor * 4);
    }
    else if(pVictim->GetEntry() == AV_NPC_GENERAL_DREK_THAR)
    {
        Herald("The Frostwolf General is dead!");
        RemoveReinforcements( 1, AV_NUM_REINFORCEMENTS );
        GiveHonorToTeam(0, m_bonusHonor * 4);
    }
    else if(pVictim->GetEntry() == AV_NPC_CAPTAIN_GALVANGAR)
    {
        RemoveReinforcements( 1, AV_POINTS_ON_KILL_CAPTAIN );
        m_LiveCaptain[1] = false;
        GiveHonorToTeam(0, m_bonusHonor * 3);
    }
    else if(pVictim->GetEntry() == AV_NPC_CAPTAIN_BALINDA_STONEHEARTH)
    {
        RemoveReinforcements( 0, AV_POINTS_ON_KILL_CAPTAIN );
        m_LiveCaptain[0] = false;
        GiveHonorToTeam(1, m_bonusHonor * 3);
    }
}

void AlteracValley::Finish(uint32 losingTeam)
{
    if(m_ended) return;

    m_ended = true;
    m_losingteam = losingTeam;
    sEventMgr.RemoveEvents(this);
    sEventMgr.AddEvent(castPtr<CBattleground>(this), &AlteracValley::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);
    SendChatMessage( CHAT_MSG_BG_SYSTEM_NEUTRAL, 0, "|cffffff00This battleground will close in 2 minutes.");

    UpdatePvPData();
}


// Static AV Loot Table
struct AVLoot
{
    uint32 ItemId;
    int8 Faction;
    float Chance;
    uint32 MinCount;
    uint32 MaxCount;
};

const static AVLoot g_avLoot[] =
{
    // Alliance Loot
    // Stormpike Soldier's Blood    // Stormpike Soldier's Meat
    { 17306, 0, 100.0f, 1, 1 },     { 17326, 0, 80.0, 1, 1 },
    // Horde Loot
    { 17423, 1, 50.0f, 1, 1 },      // Crystal Cluster
    // Global loot
    { 17422, -1, 100.0f, 1, 5 },    // Armor Scrap
    // EOF
    { 0, 0, 0},
};

void AlteracValley::HookGenerateLoot(Player* plr, Corpse* pCorpse)
{
    const AVLoot *loot_ptr = &g_avLoot[0];
    while(loot_ptr->ItemId != 0)
    {
        if( loot_ptr->Faction == -1 || loot_ptr->Faction == int8(plr->GetTeam()) )
        {
            if( Rand(loot_ptr->Chance * sWorld.getRate(RATE_DROP0)) )
            {
                if( ItemPrototype *pProto = sItemMgr.LookupEntry(loot_ptr->ItemId) )
                {
                    __LootItem li;
                    li.ffa_loot = 0;
                    li.item.displayid = pProto->DisplayInfoID;
                    li.item.itemproto = pProto;
                    if( loot_ptr->MinCount != loot_ptr->MaxCount )
                        li.StackSize = loot_ptr->MinCount+RandomUInt(loot_ptr->MaxCount - loot_ptr->MinCount);
                    else li.StackSize = loot_ptr->MaxCount;

                    li.iRandomProperty = NULL;
                    li.iRandomSuffix = NULL;
                    li.passed = false;
                    li.roll = NULL;

                    // push to vector
                    pCorpse->GetLoot()->items.push_back(li);
                }
            }
        }

        ++loot_ptr;
    }

    // add some money
    float gold = ((float(plr->getLevel()) / 2.5f)+1) * 100.0f;          // fix this later
    gold *= sWorld.getRate(RATE_MONEY);

    // set it
    pCorpse->GetLoot()->gold = float2int32(gold);
}

void AlteracValley::EventUpdateResources()
{
/*  for(uint32 i = 0; i < 2; i++)
    {
        AddReinforcements( i, m_mineControl[i] );
    }
    sEventMgr.AddEvent(this, &AlteracValley::EventUpdateResources, EVENT_BATTLEGROUND_RESOURCEUPDATE, 45000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);*/
}

void AlteracValley::EventAssaultControlPoint(uint32 x)
{
    m_nodes[x]->Capture();
}

void AlteracValley::Herald(const char *format, ...)
{
    char msgbuf[100];
    va_list ap;
    size_t msglen;
    WorldPacket data(SMSG_MESSAGECHAT, 500);

    va_start(ap, format);
    vsnprintf(msgbuf, 100, format, ap);
    va_end(ap);
    msglen = strlen(msgbuf);

    data << uint8(CHAT_MSG_MONSTER_YELL);
    data << uint32(LANG_UNIVERSAL);
    data << uint64(0);
    data << uint32(0);          // new in 2.1.0
    data << uint32(7);          // Herald
    data << "Herald";           // Herald
    data << uint64(0);
    data << uint32(msglen+1);
    data << msgbuf;
    data << uint8(0x00);
    m_mapMgr->SendPacketToPlayers(ZONE_MASK_ALL, FACTION_MASK_ALL, &data);
}

void AlteracValley::HookOnShadowSight()
{
}

void AlteracValley::SetIsWeekend(bool isweekend)
{
    m_isWeekend = isweekend;
}
