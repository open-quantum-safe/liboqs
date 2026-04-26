/*
 * fuzz_test_sig_stfl_lms.c
 *
 * Fuzz test for LMS stateful hash-based signature verification in liboqs.
 * Targets OQS_SIG_STFL_verify for the following parameter sets as specified
 * in https://github.com/open-quantum-safe/liboqs/issues/2398:
 *   - LMS_SHA256_H5_W2
 *   - LMS_SHA256_H5_W8_H5_W8
 *
 * Baseline pk / message / signature bytes are copied from the KAT .rsp files
 * under tests/KATs/sig_stfl/lms/ (count = 0 vectors). The fuzzer mutates
 * those inputs to detect crashes, memory errors, and undefined behaviour.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

/* The two target algorithms specified in issue #2398 */
static const char *TARGET_ALGS[] = {
	"LMS_SHA256_H5_W2",
	"LMS_SHA256_H5_W8_H5_W8",
};
static const size_t NUM_TARGET_ALGS = 2;

/*
 * Fuzz input layout:
 *   [0]    algorithm selector byte (index into TARGET_ALGS)
 *   [1]    field selector byte (% 3 → 0: pk/sig/msg, 1: sig/pk/msg, 2: msg/pk/sig)
 *   [2..]  fuzz bytes applied sequentially to the three fields in selected order
 */

typedef struct {
	OQS_SIG_STFL *sig;
	uint8_t *public_key;
	uint8_t *signature;
	size_t signature_len;
	uint8_t *message;
	size_t message_len;
} stfl_keypair_t;

/* One cached vector per target algorithm, initialised once */
static stfl_keypair_t cached[2];
static bool initialised = false;

/* LMS_SHA256_H5_W2.rsp */
static const char KAT_LMS_SHA256_H5_W2_PK_HEX[] =
    "000000010000000500000002ad412ccdb2962114e64225d1add892b4adf8f6b43057169755d96b6ebc59eaa37904fb719e358e6759f598593ea69f7f";
static const char KAT_LMS_SHA256_H5_W2_MSG_HEX[] =
    "54686520706f77657273206e6f742064656c65676174656420746f2074686520556e69746564205374617465732062792074686520436f6e737469747574696f6e2c206e6f722070726f6869626974656420627920697420746f20746865205374617465732c2061726520726573657276656420746f207468652053746174657320726573706563746976656c792c206f7220746f207468652070656f706c652e2e0a0a";
static const char KAT_LMS_SHA256_H5_W2_SM_HEX[] =
    "000000000000000000000002150417e62d167e4676733e92d1394e814545ed6b9b5bcdb5abfe12ef0d5e14f418017877e0ae217554001f178e1fbb78c6517a7b91a4768e2133c5c0ffc3541c1c957502a004405cb29dc9f86b37da393939e10c6050fc178853f99a31f96dee2893a203398e3cb632d761c2552e0f181eb568ce5984b89658da394dd4138df820dc6974ab9baae3253756c4c90e329464b5663e5b4cd00ec4efd4e3d4224b2fd0d0c6e0241fa222e9ca3057d903c958a9e3bba381d3b3e91406f34f368be3f40f09a4d7ae479944113e69709ea4a8eb3a0dc3c02b49822f35deaa94bf1d98335bb0a10966eaa9fc19167d9d07956f1e2cb1eeaa6ce5ef1d1860824911d8d151aa133eaa0974823d9b0ebc6218b5f1d3d39560832c303c99fa3a73f4a5c983cf80937cc18788a162e025515213225f357dcd2e2d8a9411fd4abb915892461db7d02e15c4b7f774837ca87d764d2f07e8c437465c53152691e39c65e7c6649e83ddb3568e2e31c661c649ba174f9829351817456b4e527ef6e2db153033becc31af7ffb1ec040d7f4f6ccfad654a34d7041a854cddcb8ab905356d37e4332f5632aef346eae7a49d6d3f465d3a918a532e4efe3f458bc3c2a17200e18e71bd05730f44d1d3b9490fec0e4a52d532bc987c7f29b0c4d1a1375b302a48476645f6f9d938ed7c4b9d874b496488439a299e9108be95b2c695e5049e739a9a3d14684d88b064884ea5438f2e009c0126509541457c87a3f06af3c75da61373ecda9a693b980113b22c34ba15c95696388c2e66d90b2fbb800a97a1b96b31a954e411229bbd86cac0fe668990a9c15c6181f847f2a1db288e995257b2c525e049609ee143555eb36649e119c0f427c998ffb45592ea6faffeff2a9e498428b09b6f1d3fc3a0c8b62a749c51b279be712be38f7b323455b50ac4e325703cf564025e2aa1e5876dd56a7be2ccb920dd22e475be7db8b3ff442711e9d0f3b392eda27da8a34cae12cd4f99fa17d8364d5a2a1303e996621b649a6d7105343b0a5af6dd65f2b1f13d8f36545eb65b99b8b6db5f022bc702e8bd2a1ddbc99b64a291affd1e2adcf8a01e9dd9c117e80e32f874beea046b56b44ee8cc2240d03de68c90d9f4d67e5f3aea949cf6f47d5356a6232e979329c10422193f206c74cbe2d06d290d40fb0ffcf2bafbca374a7c327006cb1b21608af282955d3259a4f4b845ac2d9b386be83e96eeda3a84a64810ae3c81b709fef15078424298917b2697d2bdd3e7f4d365b9a84bbab07c0be4d0929be663a06345d1a67360141c03c3f37891f09c24776177e34a9093fa62005187511268fb2fa80740cb751f5e9fd0d74b0f61b643d139f8ccd040c9a9a32cc740b825a2e6a5beb28a8915fbbb1bcd41c9694fcc72ed570b1e96cafaedde8adf8d5028f80eead64f9407e57ff5465da3e5e341e587cbf091f2f5bab89e95666a3fb7158718afcf685df10ea4a22ae7fe55c8f70d5d5a5973f3dde9c80f2a9eb13d58ca3485268f6e380464892f4c3c3598c81fc3d77d1ce0d3d9f50c78be78b88f6a9e17d6ec4ad9e6a86edbd5a4e5e17219fde097f422498c171ec1d0f6dbb6a2a49d34a66ae854e2650fa4a6c2833af5106472e7c787ef2da06f28f84f0f1197c7b4c4cff6ab3ceaac5ee819809ef20ee9b535252c23117da43e31ce778abfd84bf647875573217cc79c088f91ab07adc4c930a4819b936a98834ba2f9b034996d26bff2cc901317536b5481431656c083780d774b686ac5b0abd7ae2e05981c39726e6efb4ea2507b20e85cffecac815d6d9f84fa86240e6ded4dcaa67a2e2f944ac6d38f986b7268f2c240d8f5eafc0b2b67dfc11992e305e64264e19e021fd29b72dea8be8c315e3aa5f02c49fb9e77a34c48c58a28c4340ffaa7fb5b5d0c8a4aa4abd7bbf3955a0b71cbff160c5dd9a6a4ddefdf00367f414dad1c7ec0f77c1a777ec1c726a1049a2c4bc01554b95665496584cb13b28dafd7c668baa7d0c1e425e5c8cef9f75f21819a5708db01e5a5918df096424abcea2d758d4a8c5fbd96e10ba91bef727bcc47930bd45bc7d6851b21c7daf94c016e8cd10498f18ea44c6474d865a8841800dd19fe3d57611660c3729b5c2a2f1d1765a1237db7b97bcb6af56f79f275677bfee3c37fb1a0d7c2b453feea1f7adc60892a95a7e00421b4fa320ea25119fbd6af73d9049c8e109e2b1b683f5bf83933d63bd552f65fbfb806f346db3592e244acc0878c450962956f4e47bb0bced88d1ee0d18f93acdaf6bff6e6d9b451db35755c1bbb547dc4e098012823874546757f5aa94cd0bd294709da538e8c8675e96d47269451bb63d35b6f027d68aa0871b23114553e883463512890d489286886a93227ad5ba80c53f0bb4d383f15f607091a3ddf07cb199a7d8abee54097371c8ae59b354a8f86765288dc635e4f5a3f1bed2f0998f0562755035f39a4fb0f69618ebdbc3427f1fec8d1040aef11da99c0d3f0864615b1283fb2f9e66b957cf666ae08b2e11fc9153dd32b5fc43736758f65d896d253dbe16d7b16db56130851b7a0935d92d33fa75affb9d509ee73c40c9b61359ff814267e962d4aa80f6fb0ddb76dd4cbaf49fe65ad51b406a2f33bbf698a6dde309a7d7ce95f7c68ee5bccd1631f1443f0b1e823af31b2245095afdf073708fa9afa0c3b1c4606d5428cfbe3799003437b74b136b1d0d2f80848a1e5ab14fe7e9ffe02a09e21bd5d9c4eb0607bb5549f0399c2336cbc76a6e55c7f80fc0ee968c696beaa803fb020438a9ca6c89ef1e8823801e91c1cd7cbacf09c79f128f22f29c9d49355ecfe470b8df5edf9a47bdc781a3ff0ae8e5596e13bc06c610bab6121f85d54ef764e6a82a290a4ac3db0f76820d986ffeb5f19460db6246b59aa659afe6753093998876cb961b07fac8b0c0896d0931bc5843c4f545263a950523b7ad8520654564e27d0661f1cbe60812ad232b458ca36c6439750ed12df2f5d89b12f584e0023a2b8fa39c2a13da2b7497a968189f183f65bb37c5999fd6187f9979f39ec459271d67676ae157271b24ac7b3d3876840c48296096ee2730e14adb8af5ae11666d02e441f83c859aad9657c1216a9c8116d660abfb5e061f2face2"
    "f11088e661ab2046b969e72c3d23c6410c951ad210572e3cbddba77c5fb81c7594e60e1a4784d3d606b254a49f05521a0289dd14687cf8efb78a2eef2211e6021aa14f2a3d2a5a0d5013282591239e7df7991e3f44442b544e14ae382181f0790d4901b30f659f0cf9b3080a113d2bf869e44149667438b194bca40f1ce542ef7021222e0d6b7bcf40b8631e9a23b87dc1b97cd6eea407d3f7149e250c70a021b3614b5f68c065eccba5ac481fc4435f2da541a1e368d69259bc179338e641b9b14e77abe6762ae8db9a980061c85f49aa161aa92851f6f1e6a38152edac3f121326e517215cfab5746db3e026ffa2b5b4ea41d297bc4bf91f1d07c866b21425e022f0081c7284fa81c0b6b43bea2ce4d61b910aaa539b5485b79f0f766bc704eba870b33855a589ddf7525cbc07c828cf51e687f6d804e4d43535820171d38a67a8025486f0594872c996768b56177a916b13bdf622ea4d05ac608bd1f9220420733fabcbee129a4e89b243bfb4cb7cafebdbd37b1b505014ac3b9419bb51e274f8aa81f82725693341424df75c08ff6acdf622fea4988d0925ffb33466489331b00dc3432d19dddf30fd64194d4523dd3cafef94bacaf6c72b68665d1731edc8c2e9933d9302c61d576160c3c98a9fa393aa60a843238acc975f5af75532b5088a06299dfe395bf4ffdd4ba2c96ca3c7536c6986d34bdf61e01004e537748f427a7cff650918ec3c0916cdc7ba3037d078c1f96d1d77ec7560d1fa3a55443a8b2876c882a7ae2ddfeb611af3806be2f02e39a82e734728348bd90394b732da8798ea1552925e36a8ae92697bca66ec1cc4f6c2d0d60348f81f74ddae919516cb53455e4642f0782acd678e0f2daf973f866ff0f5576517038ffc52301bc53624eee8cacd1d9fdc4c74bf2afd75bd3d0c7a8010ab8d082e6edba64106cff64b5a9ac166f02b906b9b84539361a68703e43c74439306382964d3f2245a1cb9699b9aafe48922dcfec3515cf9d737d72888562b4830e5a80594ea437556ca1c7921629438c620a439196d194a862dba078524bbfd801d2cb33cd500957b84719352baae64d8163ff85cf8236c509ba495633d4972ba8094d1e293b07f7693bc939ffcf4e3979c1f077e48b6a7b2666360759b82adffc811635b860062fe55d70b1397d6dbebe2f73e722fcb90af4d98a55d4043edea799954e8950e7f6744a1e3a98b2fd48f2ff685208c8ad85becaac5ae51e187e75beaf6589773d23367379a6b46082761f247d79341d9498d210a206424d07bb88fe5018a302f4bbdb2d0ceafd4b25603fce716eddbd18ba669fd0a18c93112082b2941924ba80c5a50efaf1f391e6e0d7d6e27cc991c29f4065d75be997762e4032b62bba80be85d4c8ec05ebf54354ebed5c32ffc47caf0e7c703c38559f125521ba736225d9284e33fbab9d32ad54e3ddce4a79a6a0adb2fcd15c57281a9f150a38d8fc96a27c751a888daa65c4461b54efaf2dd2e0d3b5a6ac12027e3b9721f6bb1a32239824faf8574574845a4d649820b1a003cbe2b3e6466f47062356432d3280edc53b1c4fffc63de4fac290e4525c151ce05314c8a8b0c744acd5768936f93a7421eea8e69522ad6d188f71f4b285a535d84ca72c965bc4e40f03b09703d534b6de536b2da8bb36704ee930f1252fbaab91d5974a9d89e1411c5662d65328334a3982c31d4d0588000204acb1f5fc359302d931d8844dbf24201483041dde064ac06dc020768f40039ab61e8a4b893a1a782a49632e38d5a70120aa4ae391d69c9fe11aff5c3f4fc55153ecf9bab047e130e4585e5de2fb875ae4841d21abc580a945e4738674ee2423a237e9f03edd0bf5e395760d5ffffa1645846019fac8a58ca6741a221ea52c0c3020ca6590a7633388da3daed3ea8425ad3671f235420b00ccdaa006a0cee192855a6d8f6cc17794c4bb1da3c6d1b3b2269572bc2f9d93ba886fe30cdcdd2f105536472a580d0e9ccbaf9d2bd091cd33a2243c816da0fd4a3e8f1c01657dc236ea26cb83e05f1c6c184d9c0391535307cb945f80f9329800b39dcbb10e1ba5d13185a3050772d694429e17426136db817cffe563a77703666f1dc827d81790bcc4e9ceba15c34a28c420b6cc7df34cea563d48e9fd20a208497e0cb5a02eb25c3f67095f3f3a63d62d06ad7953ebfddbfddbb613c676394ed866622ba3323d6b1ae1b1324aa41e99170ee1e66560fa78b74051fb172f9b82e53a02be5859ca2d59a133551b7f1cfde0ec524b4fb4aefa69df632a385db308044922dce6258d71b36b14cb481e82928fafc56111ebd5969a3588ecab557b8c15654cbd559f0792b6caed5413d750867d00ef53aa597949fdb837c468c41bdeb081aae58040a50172cbf3d075a9263f4ad7da16062e80e5fe39da2e05366b79a148ae181c465aae82ecec18e98f696649bca24931a702c338f25bb2d135b95bddea4f9f92a2f925f31ee784f195d29eb4003a5b9a21cda79c6a35a579a470bdfa05ce3ed9c2a693ec92ac559691cdf22e5b074c479f58ef2395781369738f7de2c13de64090dc6575920c28a2c5164c8266e9dee906ef55c69c73fc932a64549470fcfe4da13bda91672870957d0cc002998510826d6452f7b400d09c47925eb3e4bb8f53bbd3f657d2afcd7eecc992966bb1ea7774ed15d22d931dacd5bd45993151ce89e173548d0d9324ef74be4c0cb89a1d824f1eb164821249116274a536b1b760846a4f6d1426f946d338ef04f06494a8ad0e3a59fc2269cbd411b0106e8b1245dcc3814ccf330790dd352e68a675736119d839155378f573524f5071487329e2b30405d2b0f07ad82cf2a7a3514821356f126349668f35350f3f3b71045bf60f1f608da2ebb1b067f0e9b673a1e6bf221eee8aaaae8000000051c142d7004cafdc21431a71b81e6ddc44a8ab75a3a2c8fa3a9a8cc4a781d9079063f7a97bf683473a44e5ec3877cb60d400f6a6d8a3e4fdcb02f51160f6d828ea4358ff1f2fdeb78042387d9cf0452ad8653f511d53435401a55c32b0c03a76b0edfec3b1fcb0b864da0ef3663930feab4e318cb7c271ac7b0ee8f7fa2c67838b293d94a7bddb92e05b5810f88ac6eea3182bbe37704a0190fd4658e268be368";
/* LMS_SHA256_H5_W8_H5_W8.rsp */
static const char KAT_LMS_SHA256_H5_W8_H5_W8_PK_HEX[] =
    "00000002000000050000000461a5d57d37f5e46bfb7520806b07a1b850650e3b31fe4a773ea29a07f09cf2ea30e579f0df58ef8e298da0434cb2b878";
static const char KAT_LMS_SHA256_H5_W8_H5_W8_MSG_HEX[] =
    "54686520706f77657273206e6f742064656c65676174656420746f2074686520556e69746564205374617465732062792074686520436f6e737469747574696f6e2c206e6f722070726f6869626974656420627920697420746f20746865205374617465732c2061726520726573657276656420746f207468652053746174657320726573706563746976656c792c206f7220746f207468652070656f706c652e0a";
static const char KAT_LMS_SHA256_H5_W8_H5_W8_SM_HEX[] =
    "000000010000000500000004d32b56671d7eb98833c49b433c272586bc4a1c8a8970528ffa04b966f9426eb9965a25bfd37f196b9073f3d4a232feb69128ec45146f86292f9dff9610a7bf95a64c7f60f6261a62043f86c70324b7707f5b4a8a6e19c114c7be866d488778a0e05fd5c6509a6e61d559cf1a77a970de927d60c70d3de31a7fa0100994e162a2582e8ff1b10cd99d4e8e413ef469559f7d7ed12c838342f9b9c96b83a4943d1681d84b15357ff48ca579f19f5e71f18466f2bbef4bf660c2518eb20de2f66e3b14784269d7d876f5d35d3fbfc7039a462c716bb9f6891a7f41ad133e9e1f6d9560b960e7777c52f060492f2d7c660e1471e07e72655562035abc9a701b473ecbc3943c6b9c4f2405a3cb8bf8a691ca51d3f6ad2f428bab6f3a30f55dd9625563f0a75ee390e385e3ae0b906961ecf41ae073a0590c2eb6204f44831c26dd768c35b167b28ce8dc988a3748255230cef99ebf14e730632f27414489808afab1d1e783ed04516de012498682212b07810579b250365941bcc98142da13609e9768aaf65de7620dabec29eb82a17fde35af15ad238c73f81bdb8dec2fc0e7f932701099762b37f43c4a3c20010a3d72e2f606be108d310e639f09ce7286800d9ef8a1a40281cc5a7ea98d2adc7c7400c2fe5a101552df4e3cccfd0cbf2ddf5dc6779cbbc68fee0c3efe4ec22b83a2caa3e48e0809a0a750b73ccdcf3c79e6580c154f8a58f7f24335eec5c5eb5e0cf01dcf4439424095fceb077f66ded5bec73b27c5b9f64a2a9af2f07c05e99e5cf80f00252e39db32f6c19674f190c9fbc506d826857713afd2ca6bb85cd8c107347552f30575a5417816ab4db3f603f2df56fbc413e7d0acd8bdd81352b2471fc1bc4f1ef296fea1220403466b1afe78b94f7ecf7cc62fb92be14f18c2192384ebceaf8801afdf947f698ce9c6ceb696ed70e9e87b0144417e8d7baf25eb5f70f09f016fc925b4db048ab8d8cb2a661ce3b57ada67571f5dd546fc22cb1f97e0ebd1a65926b1234fd04f171cf469c76b884cf3115cce6f792cc84e36da58960c5f1d760f32c12faef477e94c92eb75625b6a371efc72d60ca5e908b3a7dd69fef0249150e3eebdfed39cbdc3ce9704882a2072c75e13527b7a581a556168783dc1e97545e31865ddc46b3c957835da252bb7328d3ee2062445dfb85ef8c35f8e1f3371af34023cef626e0af1e0bc017351aae2ab8f5c612ead0b729a1d059d02bfe18efa971b7300e882360a93b025ff97e9e0eec0f3f3f13039a17f88b0cf808f488431606cb13f9241f40f44e537d302c64a4f1f4ab949b9feefadcb71ab50ef27d6d6ca8510f150c85fb525bf25703df7209b6066f09c37280d59128d2f0f637c7d7d7fad4ed1c1ea04e628d221e3d8db77b7c878c9411cafc5071a34a00f4cf07738912753dfce48f07576f0d4f94f42c6d76f7ce973e9367095ba7e9a3649b7f461d9f9ac1332a4d1044c96aefee67676401b64457c54d65fef6500c59cdfb69af7b6dddfcb0f086278dd8ad0686078dfb0f3f79cd893d314168648499898fbc0ced5f95b74e8ff14d735cdea968bee7400000005d8b8112f9200a5e50c4a262165bd342cd800b8496810bc716277435ac376728d129ac6eda839a6f357b5a04387c5ce97382a78f2a4372917eefcbf93f63bb59112f5dbe400bd49e4501e859f885bf0736e90a509b30a26bfac8c17b5991c157eb5971115aa39efd8d564a6b90282c3168af2d30ef89d51bf14654510a12b8a144cca1848cf7da59cc2b3d9d0692dd2a20ba3863480e25b1b85ee860c62bf51360000000500000004d2f14ff6346af964569f7d6cb880a1b66c50"
    "04917da6eafe4d9ef6c6407b3db0e5485b122d9ebe15cda93cfec582d7ab0000000a000000040703c491e7558b35011ece3592eaa5da4d918786771233e8353bc4f62323185c95cae05b899e35dffd717054706209988ebfdf6e37960bb5c38d7657e8bffeef9bc042da4b4525650485c66d0ce19b317587c6ba4bffcc428e25d08931e72dfb6a120c5612344258b85efdb7db1db9e1865a73caf96557eb39ed3e3f426933ac9eeddb03a1d2374af7bf77185577456237f9de2d60113c23f846df26fa942008a698994c0827d90e86d43e0df7f4bfcdb09b86a373b98288b7094ad81a0185ac100e4f2c5fc38c003c1ab6fea479eb2f5ebe48f584d7159b8ada03586e65ad9c969f6aecbfe44cf356888a7b15a3ff074f771760b26f9c04884ee1faa329fbf4e61af23aee7fa5d4d9a5dfcf43c4c26ce8aea2ce8a2990d7ba7b57108b47dabfbeadb2b25b3cacc1ac0cef346cbb90fb044beee4fac2603a442bdf7e507243b7319c9944b1586e899d431c7f91bcccc8690dbf59b28386b2315f3d36ef2eaa3cf30b2b51f48b71b003dfb08249484201043f65f5a3ef6bbd61ddfee81aca9ce60081262a00000480dcbc9a3da6fbef5c1c0a55e48a0e729f9184fcb1407c31529db268f6fe50032a363c9801306837fafabdf957fd97eafc80dbd165e435d0e2dfd836a28b354023924b6fb7e48bc0b3ed95eea64c2d402f4d734c8dc26f3ac591825daef01eae3c38e3328d00a77dc657034f287ccb0f0e1c9a7cbdc828f627205e4737b84b58376551d44c12c3c215c812a0970789c83de51d6ad787271963327f0a5fbb6b5907dec02c9a90934af5a1c63b72c82653605d1dcce51596b3c2b45696689f2eb382007497557692caac4d57b5de9f5569bc2ad0137fd47fb47e664fcb6db4971f5b3e07aceda9ac130e9f38182de994cff192ec0e82fd6d4cb7f3fe00812589b7a7ce515440456433016b84a59bec6619a1c6c0b37dd1450ed4f2d8b584410ceda8025f5d2d8dd0d2176fc1cf2cc06fa8c82bed4d944e71339ece780fd025bd41ec34ebff9d4270a3224e019fcb444474d482fd2dbe75efb20389cc10cd600abb54c47ede93e08c114edb04117d714dc1d525e11bed8756192f929d15462b939ff3f52f2252da2ed64d8fae88818b1efa2c7b08c8794fb1b214aa233db3162833141ea4383f1a6f120be1db82ce3630b3429114463157a64e91234d475e2f79cbf05e4db6a9407d72c6bff7d1198b5c4d6aad2831db61274993715a0182c7dc8089e32c8531deed4f7431c07c02195eba2ef91efb5613c37af7ae0c066babc69369700e1dd26eddc0d216c781d56e4ce47e3303fa73007ff7b949ef23be2aa4dbf25206fe45c20dd888395b2526391a724996a44156beac808212858792bf8e74cba49dee5e8812e019da87454bff9e847ed83db07af313743082f880a278f682c2bd0ad6887cb59f652e155987d61bbf6a88d36ee93b6072e6656d9ccbaae3d655852e38deb3a2dcf8058dc9fb6f2ab3d3b3539eb77b248a661091d05eb6e2f297774fe6053598457cc61908318de4b826f0fc86d4bb117d33e865aa805009cc2918d9c2f840c4da43a703ad9f5b5806163d7161696b5a0adc00000005d5c0d1bebb06048ed6fe2ef2c6cef305b3ed633941ebc8b3bec9738754cddd60e1920ada52f43d055b5031cee6192520d6a5115514851ce7fd448d4a39fae2ab2335b525f484e9b40d6a4a969394843bdcf6d14c48e8015e08ab92662c05c6e9f90b65a7a6201689999f32bfd368e5e3ec9cb70ac7b8399003f175c40885081a09ab3034911fe125631051df0408b3946b0bde790911e8978ba07dd56c73e7ee";

/* Decode even-length hex ASCII into out[0..out_len-1]. Returns OQS_SUCCESS or OQS_ERROR. */
static OQS_STATUS hex_decode(const char *hex, uint8_t *out, size_t out_len) {
	size_t hlen = strlen(hex);
	if (hlen != out_len * 2) {
		return OQS_ERROR;
	}
	for (size_t i = 0; i < out_len; i++) {
		unsigned char c0 = (unsigned char)hex[2 * i];
		unsigned char c1 = (unsigned char)hex[2 * i + 1];
		unsigned v0, v1;
		if (c0 >= '0' && c0 <= '9') {
			v0 = c0 - (unsigned char)'0';
		} else if (c0 >= 'A' && c0 <= 'F') {
			v0 = 10 + c0 - (unsigned char)'A';
		} else if (c0 >= 'a' && c0 <= 'f') {
			v0 = 10 + c0 - (unsigned char)'a';
		} else {
			return OQS_ERROR;
		}
		if (c1 >= '0' && c1 <= '9') {
			v1 = c1 - (unsigned char)'0';
		} else if (c1 >= 'A' && c1 <= 'F') {
			v1 = 10 + c1 - (unsigned char)'A';
		} else if (c1 >= 'a' && c1 <= 'f') {
			v1 = 10 + c1 - (unsigned char)'a';
		} else {
			return OQS_ERROR;
		}
		out[i] = (uint8_t)((v0 << 4) | v1);
	}
	return OQS_SUCCESS;
}

static void cleanup_cached(void) {
	for (size_t i = 0; i < NUM_TARGET_ALGS; i++) {
		if (cached[i].sig != NULL) {
			OQS_MEM_insecure_free(cached[i].public_key);
			OQS_MEM_insecure_free(cached[i].signature);
			OQS_MEM_secure_free(cached[i].message, cached[i].message_len);
			OQS_SIG_STFL_free(cached[i].sig);
			cached[i].sig = NULL;
		}
	}
}

typedef struct {
	const char *pk_hex;
	const char *msg_hex;
	const char *sm_hex;
} kat_hex_triple_t;

static const kat_hex_triple_t KAT_HEX[NUM_TARGET_ALGS] = {
	{ KAT_LMS_SHA256_H5_W2_PK_HEX, KAT_LMS_SHA256_H5_W2_MSG_HEX, KAT_LMS_SHA256_H5_W2_SM_HEX },
	{ KAT_LMS_SHA256_H5_W8_H5_W8_PK_HEX, KAT_LMS_SHA256_H5_W8_H5_W8_MSG_HEX, KAT_LMS_SHA256_H5_W8_H5_W8_SM_HEX },
};

/*
 * Load KAT vectors once at startup. Avoids LMS key generation cost.
 */
static OQS_STATUS init_vectors(void) {
	for (size_t i = 0; i < NUM_TARGET_ALGS; i++) {
		cached[i].sig = NULL;
		cached[i].public_key = NULL;
		cached[i].signature = NULL;
		cached[i].message = NULL;
	}

	for (size_t i = 0; i < NUM_TARGET_ALGS; i++) {
		OQS_SIG_STFL *sig = OQS_SIG_STFL_new(TARGET_ALGS[i]);
		if (sig == NULL) {
			fprintf(stderr, "ERROR: %s not enabled at compile-time.\n",
			        TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}
		cached[i].sig = sig;

		cached[i].public_key = OQS_MEM_malloc(sig->length_public_key);
		cached[i].signature = OQS_MEM_malloc(sig->length_signature);
		cached[i].signature_len = sig->length_signature;
		cached[i].message_len = strlen(KAT_HEX[i].msg_hex) / 2;
		cached[i].message = OQS_MEM_malloc(cached[i].message_len);

		if (!cached[i].public_key || !cached[i].message || !cached[i].signature) {
			fprintf(stderr, "ERROR: OQS_MEM_malloc failed for %s.\n",
			        TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}

		if (hex_decode(KAT_HEX[i].pk_hex, cached[i].public_key,
		               sig->length_public_key) != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: KAT pk hex length mismatch for %s.\n",
			        TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}
		if (hex_decode(KAT_HEX[i].msg_hex, cached[i].message,
		               cached[i].message_len) != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: KAT msg hex for %s.\n", TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}
		if (hex_decode(KAT_HEX[i].sm_hex, cached[i].signature,
		               sig->length_signature) != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: KAT sm hex length mismatch for %s.\n",
			        TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}
	}
	return OQS_SUCCESS;
}

static OQS_STATUS fuzz_sig_stfl_lms(const uint8_t *data, size_t data_len) {
	if (data_len < 1) {
		return OQS_ERROR;
	}

	/* Byte 0: algorithm selector */
	size_t alg_idx = data[0] % NUM_TARGET_ALGS;
	stfl_keypair_t *kp = &cached[alg_idx];
	OQS_SIG_STFL *sig = kp->sig;

	const uint8_t *fuzz_data = data + 1;
	size_t fuzz_len = data_len - 1;

	if (fuzz_len < 1) {
		return OQS_ERROR;
	}

	/* Byte 1: field selector — determines which field is mutated first */
	size_t field_selector = fuzz_data[0] % 3;
	fuzz_data++;
	fuzz_len--;

	size_t pk_len = sig->length_public_key;
	size_t sig_len = kp->signature_len;
	size_t msg_len = kp->message_len;

	uint8_t *mutated_pk = OQS_MEM_malloc(pk_len);
	uint8_t *mutated_sig = OQS_MEM_malloc(sig_len);
	uint8_t *mutated_msg = OQS_MEM_malloc(msg_len);

	if (!mutated_pk || !mutated_sig || !mutated_msg) {
		OQS_MEM_insecure_free(mutated_pk);
		OQS_MEM_insecure_free(mutated_sig);
		OQS_MEM_insecure_free(mutated_msg);
		return OQS_ERROR;
	}

	/* Start from KAT values, then overwrite with fuzz bytes in selected order */
	memcpy(mutated_pk, kp->public_key, pk_len);
	memcpy(mutated_sig, kp->signature, sig_len);
	memcpy(mutated_msg, kp->message, msg_len);

	/*
	 * field_selector % 3 controls which field is overwritten first:
	 *   0 → pk, sig, msg   1 → sig, pk, msg   2 → msg, pk, sig
	 */
	uint8_t *fp[3];
	size_t fl[3];
	switch (field_selector) {
	case 1:
		fp[0] = mutated_sig;
		fl[0] = sig_len;
		fp[1] = mutated_pk;
		fl[1] = pk_len;
		fp[2] = mutated_msg;
		fl[2] = msg_len;
		break;
	case 2:
		fp[0] = mutated_msg;
		fl[0] = msg_len;
		fp[1] = mutated_pk;
		fl[1] = pk_len;
		fp[2] = mutated_sig;
		fl[2] = sig_len;
		break;
	default:
		fp[0] = mutated_pk;
		fl[0] = pk_len;
		fp[1] = mutated_sig;
		fl[1] = sig_len;
		fp[2] = mutated_msg;
		fl[2] = msg_len;
		break;
	}

	size_t offset = 0;
	for (size_t i = 0; i < 3; i++) {
		if (offset < fuzz_len) {
			size_t copy = (fuzz_len - offset) < fl[i] ? (fuzz_len - offset) : fl[i];
			memcpy(fp[i], fuzz_data + offset, copy);
			offset += copy;
		}
	}

	/*
	 * Call OQS_SIG_STFL_verify with mutated inputs.
	 * We expect OQS_ERROR for invalid inputs — a crash here is a bug.
	 */
	OQS_SIG_STFL_verify(sig, mutated_msg, msg_len, mutated_sig, sig_len,
	                    mutated_pk);

	OQS_MEM_insecure_free(mutated_pk);
	OQS_MEM_insecure_free(mutated_sig);
	OQS_MEM_secure_free(mutated_msg, msg_len);

	return OQS_SUCCESS;
}

int LLVMFuzzerInitialize(int *argc, char ***argv) {
	(void)argc;
	(void)argv;
	OQS_init();
	if (OQS_SUCCESS != init_vectors()) {
		return -1;
	}
	initialised = true;
	return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	if (!initialised) {
		return -1;
	}
	fuzz_sig_stfl_lms(data, size);
	return 0;
}
