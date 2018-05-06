--********************************************************************************************
--* FPGASIKE: a speed optimized hardware implementation of the 
--*           Supersingular Isogeny Key Encapsulation scheme
--*
--*    Copyright (c) Brian Koziel and Reza Azarderakhsh
--*
--********************************************************************************************* 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

use IEEE.NUMERIC_STD.ALL;
use STD.TEXTIO.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;

entity sike_p751_tb is
--  Port ( );
end sike_p751_tb;

architecture Behavioral of sike_p751_tb is

    constant PERIOD : time := 5000 ps;
    
    constant TEST_WITHOUT_CFK : boolean := true;
    constant TEST_WITH_CFK : boolean := false;

component sike_p751 is
    Port ( clk                     : in  std_logic;
           rst                     : in  std_logic;
           sike_cmd_i              : in  std_logic_vector(2 downto 0);
           iso_cmd_i               : in  std_logic_vector(2 downto 0);
           reg_sel_i               : in  std_logic_vector(7 downto 0);
           wr_input_sel_i          : in  std_logic;
           wr_op_sel_i             : in  std_logic_vector(1 downto 0);
           wr_word_sel_i           : in  std_logic_vector(3 downto 0);
           wr_en_i                 : in  std_logic;
           rd_reg_i                : in  std_logic;
           rd_op_sel_i             : in  std_logic_vector(1 downto 0);
           rd_word_sel_i           : in  std_logic_vector(3 downto 0);
           buffer_xor_i            : in  std_logic;
           keccak_clear_i          : in  std_logic;
           keccak_din_i            : in  std_logic_vector(7 downto 0);
           keccak_din_valid_byte_i : in  std_logic;
           keccak_word_cnt_i       : in  std_logic_vector(2 downto 0);
           keccak_word_valid_i     : in  std_logic;
           data_i                  : in  std_logic_vector (63 downto 0);
           data_o                  : out std_logic_vector (63 downto 0);
           busy_o                  : out std_logic
    );
end component sike_p751; 

    signal clk     : std_logic := '0';
    signal rst     : std_logic := '0';
    signal sike_cmd_i : std_logic_vector(2 downto 0) := (others => '0');
    signal iso_cmd_i : std_logic_vector(2 downto 0) := (others => '0');
    signal reg_sel_i : std_logic_vector(7 downto 0) := (others => '0');
    signal wr_input_sel_i : std_logic := '0';
    signal wr_op_sel_i : std_logic_vector(1 downto 0) := (others => '0');
    signal wr_word_sel_i : std_logic_vector(3 downto 0) := (others => '0');
    signal wr_en_i : std_logic := '0';
    signal rd_reg_i : std_logic := '0';
    signal rd_op_sel_i : std_logic_vector(1 downto 0) := (others => '0');
    signal rd_word_sel_i : std_logic_vector(3 downto 0) := (others => '0');
    signal buffer_xor_i : std_logic := '0';
    signal keccak_clear_i : std_logic := '0';
    signal keccak_din_i : std_logic_vector(7 downto 0) := (others => '0');
    signal keccak_din_valid_byte_i : std_logic := '0';
    signal keccak_word_cnt_i : std_logic_vector(2 downto 0) := (others => '0');
    signal keccak_word_valid_i : std_logic := '0';
    signal data_i : std_logic_vector(63 downto 0) := (others => '0');
    signal data_o : std_logic_vector(63 downto 0);
    signal busy_o : std_logic;
    
    signal list_mem : std_logic := '0';
    signal list_mem_done : std_logic := '0';
    
  
    
    type alice_key_array is array(0 to 1) of std_logic_vector(371 downto 0); 
    constant alice_keys_hex : alice_key_array :=(
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    --x"6e76c4125a57cef20fbd1c4603a4d3b6357e7ca39bf385feca2fe3fb5069ef45a75e22a9a7f0fb7df53e608835419",
    --x"836156252c31402fa6c678c18c10e61e56fc2d43e77db9e3a670141ffbe18ba493da9c65f613fcf7ba13755e2ae6e"
    );
    
    type bob_key_array is array(0 to 1) of std_logic_vector(378 downto 0);
    constant bob_keys_hex : bob_key_array :=(
    "001" & x"02505c57d33805e406218589720aab8ffb2b878b9efa90dc137d406760550e21f856b9593be000081451d479ed2686",
    "011" & x"0303f6c2d77af80a7997ff1879323f72f60ba4eab0f165f76cba6ca646b343308546cbd51e72b88572f21c53713200"
    );
    
    
    type mont_constants_array is array(0 to 6) of std_logic_vector(767 downto 0);
    constant mont_array_hex : mont_constants_array :=(
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --0
    x"00002034557c11c7e3dadec0d08a4732f53a95f7469b516fef62cf3052282f124ad4c9cbd81067123826d2f56c0f0eae22a000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002", --mont 1
    x"00004068aaf8238fc7b5bd81a1148e65ea752bee8d36a2dfdec59e60a4505e2495a99397b020ce24704da5ead81e1d5c454000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004", --mont 2
    x"000044ebae3ac01f8219f99c1d39c462c19d3a99de6f41e506509b95723718e54af4b34a0e7e850bb42228d097b4bcb9712380c62bf35f1f1ee7fc814932cca8904f8751f40bfe2082a2e7065e36941472e3fd8edb010161a696452a23304644", --R^2
    x"000040000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --4^-1
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001", --1
    x"000051542ba67393490ea7e54b82cecb39fccec74af19157940242ba160531f6e6671facfc6a9df66cfc5b3b3e61df6be1100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000d" --mont 6
    );
    
    type params_array is array(0 to 29) of std_logic_vector(767 downto 0);
    constant params_hex : params_array :=(
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --A.a Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --A.b Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --B.a
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001", --B.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --A24.a Unused
    x"0000101a2abe08e3f1ed6f60684523997a9d4afba34da8b7f7b1679829141789256a64e5ec0833891c13697ab6078757115000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001", --A24.b Unused
    x"000028849BC0D81E01993137A5B63D6E633C4E97AB4FF118CCF63DFE623092AC86B6D4A9B751797CBA1A177500E9EB5AF7852B7DF02C334844D652EFC4729178A1DBAD8CA47BB7E757C6D43B799811A63BEBE649C18101F03AD752CDCD73BF66", --PAx.a
    x"000054921C31F0DC9531CB890FC5EC66DF2E7F0D55761363C6E375DA69B0682CABE5C0FFFCBE6E1AD46563F042FA06B9F207FCF3CDD2673652828FF50C3F7B755C0BE072950D16CA747C146775C0267A401FFC738B03A49E9A36B39572AFB363", --PAx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --PAy.a Unused
    x"00003bc90159e18718d03a0fc29b286f802c4520e821ad32132150e4530b4b8503138aad9e0780325b0f35c338f70355f587a76b4bf75b88ba8a751bd86908c32cb01fe9208292f8ed7c157a9dcb510427e481a1df00dce61c24ccd42e9041c2", --PAy.b Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --QAx.a
    x"00003E82027A38E9429C8D36FF46BCC93FA23F89F6BE06D2B1317AD90438621783FDB7A4AD3E83E86CAE096D5DB822C98E561E008FA0E3F3B9AC2F40C56D6FA4A58A20449AF1F1335661D14AB7347693632646086CE3ACD54B0346F5CCE233E9", --QAx.b
    x"00003bc90159e18718d03a0fc29b286f802c4520e821ad32132150e4530b4b8503138aad9e0780325b0f35c338f70355f587a76b4bf75b88ba8a751bd86908c32cb01fe9208292f8ed7c157a9dcb510427e481a1df00dce61c24ccd42e9041c2", --QAy.a Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --QAy.b Unused
    x"00000F9C4AFCDA809C3358B096B250C69B20310FDF2EF631711AA4EFEC49A4E76483F320B793F2EBC63365EED14AA3F6EA33FEB56796F011BA6C6DFB4D0A00AAC4D2786646D914AD026CBB4A592EC74B5485372E51382D44528DD491B83D9547", --QPAx.a
    x"000022A0B5A35A2B0C56135A7CEC5CFB97964A7C6226FE909F374362A8ECA3AB14A1B7B0C87AC875DCE5888D83B623BF0011A4AC138F62EF6B2D2D84F636548A9F920F238336E5A36E45E4055940E3C94385B8FC5374396432EEF2AE178CEFDD", --QPAx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --QPAy.a Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --QPAy.b Unused
    x"00002B88A03360B3389547732C9140C05DEA6516881FE108211BE887CC43FCB80C06A1D86FF5457D3BB7DB936394EC33821AA39333A60AF84B537974CFA0BA8287D699D2BF79BA559026C64A6ED610501D2357C10B9A6C8F837424922275ACBF", --PBx.a
    x"000005FD1A3C4DD0F630974196FED3519152BC7098B9E2B121ECA46BD10A5CC9F4BCC6C689B8E4C063B3798075FCEE6EDAA9EB108B3CD00495CF04DD8CE4A08FBE685A127D40E45F4CF45098A578DEB44368699394C43BFC9BC5E00052F78E8D", --PBx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --PBy.a Unused
    x"00003d72a33e8e5964eeab877db30d816649f8974d76410023b8494622b012ae248e6a89e10072ee0d7ee9b68399648093c849d8348f9b629361f99ebd204d5014964b222d95fc81619d1eb54c3e8010f57a06cfe456b25dbe012e1eb8ed8c1c", --PBy.b Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --QBx.a
    x"00002F1D80EF06EF960A01AB8FF409A2F8D5BCE859ED725DE145FE2D525160E0A3AD8E17B9F9238CD5E69CF26DF237429BD3778659023B9ECB610E30288A7770D3785AAAA4D646C576AECB94B919AEEDD9E1DF566C1D26D376ED2325DCC93103", --QBx.b
    x"00003d72a33e8e5964eeab877db30d816649f8974d76410023b8494622b012ae248e6a89e10072ee0d7ee9b68399648093c849d8348f9b629361f99ebd204d5014964b222d95fc81619d1eb54c3e8010f57a06cfe456b25dbe012e1eb8ed8c1c", --QBy.a Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --QBy.b Unused
    x"00006D91393A57DBF47FD6DCF841F17ECD719CAE1D33C6832A75B0F168855BCC38D2A4792DFF9BC86DEACA10B1AA808D539B167D73BBA32168687FA3F85AE93A1ADDE5BD1FD5B681DCC6C34454D4496976C22D80C95E42B12576FC0FB4074B9F", --QPBx.a
    x"0000077B3BB69009428A327D43CA60169715F547454F88CD017B32DF58A7252C2B3C3D00D52CCD3133D54041D8BCAEA291F2057202328712CD395575CD7CCD3CE70C0A1EBF633BA946559458878F41F9FDD1727E2C31125B2FE5B71306704829", --QPBx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --QPBy.a Unused
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"); --QPBy.b Unused

    type r2_array is array(0 to 35) of std_logic_vector(767 downto 0); --For KAT0 only
    constant round2_params_hex : r2_array :=(
    x"00004213c3da5cf8484d11d0fecfbce4946c8d268e609757302269c2a6171505e8d666af32cf49bc7575970b64f5e9dee316e4eaeb313f74c2d44749a250feca33f56e4c6a941bfe5c9945d22f6a6c47cabf2705b3deb763bfe247e39618e9ca", --EA.a
    x"00005faead67227824fb121965be89a152b3766180b868f000768c47793bc1b2d89905d1b835a769123cb75e149d6b80756edb7e6e1ce4f95d6a9837d7a310b287323b06dd8d6a30399a137a0de7f183d675b7480f664508d2d4a2dab3b794be", --EA.b
    x"000020FD8652ECAD697960C78133B9F63B87A877020F2629F2F1999C45AFE3FB64802FAE1ABE499B7A153F9F27C9179269B9B759E9F056B1B775BE1B4DEEE162F3699A50499CD3DDDA49AAD6FF175A0B87E3AFE432FCC5FF4B93C9BAEAD39D2A", --j(EAB).a
    x"0000139A5D9624369164A5C8DBFCAEAF2AE0A9A661DBF5BE24EE19BDF92A6ACC19C2C040B79F3A94F421319F8446E74327749133751B1F736E6C52AEEF842E64530AC2A6670714E6C6FEA0C0AEC0906042E42D95DCB3A02E050F919BE76462E4", --j(EAB).b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --EA24.a
    x"0000101a2abe08e3f1ed6f60684523997a9d4afba34da8b7f7b1679829141789256a64e5ec0833891c13697ab6078757115000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001", --EA24.b
    x"0000694e92cf22c72fb55e0e9196a696fd94e86188d242edcd579846a26c5c060288a4f0e710248e4a6aafaa6bf03dd0bbd866f2560b98d91ddd8b976bb59e620f0f5c9d7bf788f22e1dd06770d62a7f646bb8ab4ddbefe533e59f386ca1a9a1", --EB.a
    x"00000887ce77dfcf15e5f65b7dff56a4eeb322c722605520600c61da129ade30f07861e28023a649691db58ce58b52d5effb19d046ec72babe1750006b8a4a271d50ed4980ca8184da889fc916d2622282cc3f4acdc037a18afad13c3b02d4cd", --EB.b
    x"000020FD8652ECAD697960C78133B9F63B87A877020F2629F2F1999C45AFE3FB64802FAE1ABE499B7A153F9F27C9179269B9B759E9F056B1B775BE1B4DEEE162F3699A50499CD3DDDA49AAD6FF175A0B87E3AFE432FCC5FF4B93C9BAEAD39D2A", --j(EBA).a
    x"0000139A5D9624369164A5C8DBFCAEAF2AE0A9A661DBF5BE24EE19BDF92A6ACC19C2C040B79F3A94F421319F8446E74327749133751B1F736E6C52AEEF842E64530AC2A6670714E6C6FEA0C0AEC0906042E42D95DCB3A02E050F919BE76462E4", --j(EBA).b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --EB24.a
    x"0000101a2abe08e3f1ed6f60684523997a9d4afba34da8b7f7b1679829141789256a64e5ec0833891c13697ab6078757115000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001", --EB24.b
    x"000049939a937cc243e46cd5d6e1b4efb74279e368f2506c84d02c8838734044ea43e850b1ed9725dc0989bb907bc8b0a94fc861a433b08d89acb4b5a80970815df52ec63245e76c125e17c196a78ea335c7705aa6ee52243ea249f021e70bc1", --phiPBx.a
    x"00003d7fb9b77735136f1d03edd31804226b7b20adb13574983368fa6eb82feda2c828072124b2ac1dadb15133905ec522f5824b8c8c79986b67f5b617f6c588b6f3db853ae2ed5b4e306f7ebbc6327210164a5affcfc3d53368b0f336253036", --phiPBx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --phiPBy.a
    x"00003bc90159e18718d03a0fc29b286f802c4520e821ad32132150e4530b4b8503138aad9e0780325b0f35c338f70355f587a76b4bf75b88ba8a751bd86908c32cb01fe9208292f8ed7c157a9dcb510427e481a1df00dce61c24ccd42e9041c2", --phiPBy.b
    x"00003f853c360a21d0fbee894c32274ed17c72f8780e051fb1ed54c71d64fd9f1133366bb0c68c85fc666fda1b938c765a199d39dd0d3a973025eadfcbc330c22e4bffc92e9fcd9126a9f446ae846f37065b37e8d4ae6119740d8e83bf702612", --phiQBx.a
    x"000037bb10ddc7e5262eefd9ed52649c7afed747491af7acf5e879795e01337cc797f21a60ae5b4384c4e2a4204e406bd8134bda30496461564a1fd3323dea48e1fadc523b6df5d7fcce5eaef5decea32e6deff883516d6ff3085cfc947f2eb5", --phiQBx.b
    x"00003bc90159e18718d03a0fc29b286f802c4520e821ad32132150e4530b4b8503138aad9e0780325b0f35c338f70355f587a76b4bf75b88ba8a751bd86908c32cb01fe9208292f8ed7c157a9dcb510427e481a1df00dce61c24ccd42e9041c2", --phiQBy.a
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --phiQBy.b
    x"00006bed91b243aaf49d3454c1211cb3f9f392e4f226de20420d73e03787f32ff57d180dcf67722e00d7e5b89b9bbd5297216fe136458060f55382eb5598d2f364be3539e2545868c8be5d0c7e29c88c366f7cb59713acfeceee4b144244b8b9", --phiQPBx.a
    x"0000012f6fcbae1f1bfa45101d68a067aac957054e649b2632cfe7e5fba95a6c30d137ddce8146685a5dc54a2a20d742cd30e2295ecf2054b6c70ae53b8e8c490f766f56fd7cc59f1dea4fb36c571419c1db81e89c4f0c6d18bec76f2292babd", --phiQPBx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --phiQPBy.a not used
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --phiQPBy.b not used
    x"000021841470ccefbbc84c211cd041e8fc0102f545a0af3f0875654de98a8d9994cce28c32d2f0ad1a08c310f1bed99136de365103cf9c2a234889fecb0854d7cfba04a0907f0d312055ec4272f99fff11f1e50c85f04dca1dc876bed7c3f2fa", --phiPAx.a
    x"000035ea393834a77e05064990b64ee70cba36c16e8bdbdc9b781b750c421b127e0a61dd84e620a549bf065ff049054c90ffb58cd62012880cef5284b4bbbea451cc951a14b09ff34db7a715d00a9657493a36b4301b1aa1b904556668e2557c", --phiPAx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --phiPAy.a
    x"00003bc90159e18718d03a0fc29b286f802c4520e821ad32132150e4530b4b8503138aad9e0780325b0f35c338f70355f587a76b4bf75b88ba8a751bd86908c32cb01fe9208292f8ed7c157a9dcb510427e481a1df00dce61c24ccd42e9041c2", --phiPAy.b
    x"000037b604b67aa8212c47e7c2717b9b5ca700f96930b6afa739a975e9c7b46b5df8ec90b2b7808e84efc8872c9faa290fe001b9d7c9da5d041d1b1a9070d99d0be1935f4609fcc2aa9f42f71f3290c78a16e1cac429cc9b807a1d689a34c575", --phiQAx.a
    x"00005ca5aed402e06d2154f2132d2d21b63e26faa24d5444cc82b318202b9c4a3d3d8c34239db1782d06c1f3c2154a1114d96a1946a81d91b737a5f30c7f3918f9907147550b4ef7aff08c902c8ae7d96f6f060bafbe89daa0764c201805a75e", --phiQAx.b
    x"00003bc90159e18718d03a0fc29b286f802c4520e821ad32132150e4530b4b8503138aad9e0780325b0f35c338f70355f587a76b4bf75b88ba8a751bd86908c32cb01fe9208292f8ed7c157a9dcb510427e481a1df00dce61c24ccd42e9041c2", --phiQAy.a
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --phiQAy.b
    x"0000576c172398783603445b3727abc64ac0b703d889f891630c3b77b3fda0ae6fc00d28da23bacf79a7dc877cfb1c5d239d70260633941c49609b35b208b99800a96b34f3ea839f3aed7b24bffe558117dd9889d7d491037d97a625bdb65b01", --phiQPAx.a
    x"000045338140269ddd3c2ed2bb2eb75ae3f15fe5cfa6a173a8274963c4fe311dbede26d5677fe6d42f778c21f3367a215fd1b252dd99c3c9f509a8e38c0f558735748fbd5ceba1b4bb9955f3812b8f60e6a38f6d6319a88112714a97c5be4e2d", --phiQPAx.b
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --phiQPAy.a not used
    x"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"); --phiQPAy.b not used); --QPBy.b not used
    
    type secret_msg_alice_array is array(0 to 1) of std_logic_vector(255 downto 0);
    constant secret_msg_alice_array_hex : secret_msg_alice_array :=(
    x"2d7f73369973cd2d0348b1cc251ad82fdd1a6bdbe4106d0caa9476b0a035997c",
    x"d926f38a65787a0e173fa91081ee6c40f33731730bfca67b1c8c1d2a49930bd6");
 
    type secret_msg_bob_array is array(0 to 1) of std_logic_vector(255 downto 0);
    constant secret_msg_bob_array_hex : secret_msg_bob_array :=(
    x"4d42999c83df029efefcc84fab77ad7c78568d676708ca46217761a93b565562",
    x"e5fdcb363c0aa3bcf8a6b7b17619a9f73a70c40de7244c53c8dc54b5d1610c0b");   
 
    signal public_key : std_logic_vector(564*8-1 downto 0) := (others => '0');
    signal secret_key : std_logic_vector(644*8-1 downto 0) := (others => '0');
    signal cipher_text : std_logic_vector(596*8-1 downto 0) := (others => '0');
    signal shared_secretA : std_logic_vector(24*8-1 downto 0) := (others => '0');
    signal shared_secretB : std_logic_vector(24*8-1 downto 0) := (others => '0');  
    
    type public_key_array is array(0 to 1) of std_logic_vector(564*8-1 downto 0);
    constant public_key_kat : public_key_array :=(
    x"576c172398783603445b3727abc64ac0b703d889f891630c3b77b3fda0ae6fc00d28da23bacf79a7dc877cfb1c5d239d70260633941c49609b35b208b99800a96b34f3ea839f3aed7b24bffe558117dd9889d7d491037d97a625bdb65b0145338140269ddd3c2ed2bb2eb75ae3f15fe5cfa6a173a8274963c4fe311dbede26d5677fe6d42f778c21f3367a215fd1b252dd99c3c9f509a8e38c0f558735748fbd5ceba1b4bb9955f3812b8f60e6a38f6d6319a88112714a97c5be4e2d37b604b67aa8212c47e7c2717b9b5ca700f96930b6afa739a975e9c7b46b5df8ec90b2b7808e84efc8872c9faa290fe001b9d7c9da5d041d1b1a9070d99d0be1935f4609fcc2aa9f42f71f3290c78a16e1cac429cc9b807a1d689a34c5755ca5aed402e06d2154f2132d2d21b63e26faa24d5444cc82b318202b9c4a3d3d8c34239db1782d06c1f3c2154a1114d96a1946a81d91b737a5f30c7f3918f9907147550b4ef7aff08c902c8ae7d96f6f060bafbe89daa0764c201805a75e21841470ccefbbc84c211cd041e8fc0102f545a0af3f0875654de98a8d9994cce28c32d2f0ad1a08c310f1bed99136de365103cf9c2a234889fecb0854d7cfba04a0907f0d312055ec4272f99fff11f1e50c85f04dca1dc876bed7c3f2fa35ea393834a77e05064990b64ee70cba36c16e8bdbdc9b781b750c421b127e0a61dd84e620a549bf065ff049054c90ffb58cd62012880cef5284b4bbbea451cc951a14b09ff34db7a715d00a9657493a36b4301b1aa1b904556668e2557c",
    x"5d757c74bb42df96f9eb044008e72606d6c5a0aaf0859482a666c98121f103ffc383ae1ddc8d3cafcc250dfa848806c62096ffe6c3b6441cfe833b5c9ca09f38b6886f24c78b9fe93808026748a919953329c45a9783a53d2c2480c4492542d23ec27a705242065a638d7a13857a3ebb16ec1641783ee52644f1d3f0f6bc02623cf1ab4aa93bb5c6a04a809f13404232e8e928d14491f530093f842724d20bbcd33b40ae6bc4c7537d193caac05ea072ef16a4525e3108fa4234c20b160cd91244afb4fe8f4cfca3456b4ef472f2eda44e177f65e28d331bf2ff999b07df9c23fc4908e2fe3c3919e039348b75d5774db3e6ed2d75340340945e9819257351dd32ab979c494e180a437dc02f744dbca34b8aec6c359ce96e1fce5135407e9462fb30eee6a52de8d1df5d8bd253747cbc71e94f40f09ac65244fad4228e43b24fb4e2ade9cbed3af92612dab097f579853297c0fc1b9ea6dd1bfd96b3d79f891181dab860160b7e839c20dbd4b5a8ee8ad69097831c55a41c266636c10931600a02beeca4a81d42d05c057264759e5d15293c8ba0a844ce365a621edbe477912fb13471ab2d6a9d77c3d6497ef5baa19c519b7b4a00a5f0bbf465d589fd0ffab7ac098b865550ea092d8096d761fa9287d54fae67c0904677e6879c10d5622029fb0cba3f861eaf51714e8bd13e40be7c7332127ac6666c8ac5d5b8b3bc402e633ce5ddc590cd667fe7e8de54356c1fc0c983ed902a2bc7b754662e8648e348b680115ac079cfee1edbd56496dc6d0c40ff296eac");    
    
    type secret_key_array is array(0 to 1) of std_logic_vector(644*8-1 downto 0);
    constant secret_key_kat : secret_key_array :=(
    x"576c172398783603445b3727abc64ac0b703d889f891630c3b77b3fda0ae6fc00d28da23bacf79a7dc877cfb1c5d239d70260633941c49609b35b208b99800a96b34f3ea839f3aed7b24bffe558117dd9889d7d491037d97a625bdb65b0145338140269ddd3c2ed2bb2eb75ae3f15fe5cfa6a173a8274963c4fe311dbede26d5677fe6d42f778c21f3367a215fd1b252dd99c3c9f509a8e38c0f558735748fbd5ceba1b4bb9955f3812b8f60e6a38f6d6319a88112714a97c5be4e2d37b604b67aa8212c47e7c2717b9b5ca700f96930b6afa739a975e9c7b46b5df8ec90b2b7808e84efc8872c9faa290fe001b9d7c9da5d041d1b1a9070d99d0be1935f4609fcc2aa9f42f71f3290c78a16e1cac429cc9b807a1d689a34c5755ca5aed402e06d2154f2132d2d21b63e26faa24d5444cc82b318202b9c4a3d3d8c34239db1782d06c1f3c2154a1114d96a1946a81d91b737a5f30c7f3918f9907147550b4ef7aff08c902c8ae7d96f6f060bafbe89daa0764c201805a75e21841470ccefbbc84c211cd041e8fc0102f545a0af3f0875654de98a8d9994cce28c32d2f0ad1a08c310f1bed99136de365103cf9c2a234889fecb0854d7cfba04a0907f0d312055ec4272f99fff11f1e50c85f04dca1dc876bed7c3f2fa35ea393834a77e05064990b64ee70cba36c16e8bdbdc9b781b750c421b127e0a61dd84e620a549bf065ff049054c90ffb58cd62012880cef5284b4bbbea451cc951a14b09ff34db7a715d00a9657493a36b4301b1aa1b904556668e2557c0102505c57d33805e406218589720aab8ffb2b878b9efa90dc137d406760550e21f856b9593be000081451d479ed26862d7f73369973cd2d0348b1cc251ad82fdd1a6bdbe4106d0caa9476b0a035997c",
    x"5d757c74bb42df96f9eb044008e72606d6c5a0aaf0859482a666c98121f103ffc383ae1ddc8d3cafcc250dfa848806c62096ffe6c3b6441cfe833b5c9ca09f38b6886f24c78b9fe93808026748a919953329c45a9783a53d2c2480c4492542d23ec27a705242065a638d7a13857a3ebb16ec1641783ee52644f1d3f0f6bc02623cf1ab4aa93bb5c6a04a809f13404232e8e928d14491f530093f842724d20bbcd33b40ae6bc4c7537d193caac05ea072ef16a4525e3108fa4234c20b160cd91244afb4fe8f4cfca3456b4ef472f2eda44e177f65e28d331bf2ff999b07df9c23fc4908e2fe3c3919e039348b75d5774db3e6ed2d75340340945e9819257351dd32ab979c494e180a437dc02f744dbca34b8aec6c359ce96e1fce5135407e9462fb30eee6a52de8d1df5d8bd253747cbc71e94f40f09ac65244fad4228e43b24fb4e2ade9cbed3af92612dab097f579853297c0fc1b9ea6dd1bfd96b3d79f891181dab860160b7e839c20dbd4b5a8ee8ad69097831c55a41c266636c10931600a02beeca4a81d42d05c057264759e5d15293c8ba0a844ce365a621edbe477912fb13471ab2d6a9d77c3d6497ef5baa19c519b7b4a00a5f0bbf465d589fd0ffab7ac098b865550ea092d8096d761fa9287d54fae67c0904677e6879c10d5622029fb0cba3f861eaf51714e8bd13e40be7c7332127ac6666c8ac5d5b8b3bc402e633ce5ddc590cd667fe7e8de54356c1fc0c983ed902a2bc7b754662e8648e348b680115ac079cfee1edbd56496dc6d0c40ff296eac030303f6c2d77af80a7997ff1879323f72f60ba4eab0f165f76cba6ca646b343308546cbd51e72b88572f21c53713200d926f38a65787a0e173fa91081ee6c40f33731730bfca67b1c8c1d2a49930bd6");

    type cipher_text_array is array(0 to 1) of std_logic_vector(596*8-1 downto 0);
    constant cipher_text_kat : cipher_text_array :=(
    x"a026c563918dea6323f6f62fcca20e6d3ef1ddc408c900d7a193e985e64edf5b6bed91b243aaf49d3454c1211cb3f9f392e4f226de20420d73e03787f32ff57d180dcf67722e00d7e5b89b9bbd5297216fe136458060f55382eb5598d2f364be3539e2545868c8be5d0c7e29c88c366f7cb59713acfeceee4b144244b8b9012f6fcbae1f1bfa45101d68a067aac957054e649b2632cfe7e5fba95a6c30d137ddce8146685a5dc54a2a20d742cd30e2295ecf2054b6c70ae53b8e8c490f766f56fd7cc59f1dea4fb36c571419c1db81e89c4f0c6d18bec76f2292babd3f853c360a21d0fbee894c32274ed17c72f8780e051fb1ed54c71d64fd9f1133366bb0c68c85fc666fda1b938c765a199d39dd0d3a973025eadfcbc330c22e4bffc92e9fcd9126a9f446ae846f37065b37e8d4ae6119740d8e83bf70261237bb10ddc7e5262eefd9ed52649c7afed747491af7acf5e879795e01337cc797f21a60ae5b4384c4e2a4204e406bd8134bda30496461564a1fd3323dea48e1fadc523b6df5d7fcce5eaef5decea32e6deff883516d6ff3085cfc947f2eb549939a937cc243e46cd5d6e1b4efb74279e368f2506c84d02c8838734044ea43e850b1ed9725dc0989bb907bc8b0a94fc861a433b08d89acb4b5a80970815df52ec63245e76c125e17c196a78ea335c7705aa6ee52243ea249f021e70bc13d7fb9b77735136f1d03edd31804226b7b20adb13574983368fa6eb82feda2c828072124b2ac1dadb15133905ec522f5824b8c8c79986b67f5b617f6c588b6f3db853ae2ed5b4e306f7ebbc6327210164a5affcfc3d53368b0f336253036",
    x"44888ea1dbc0ba4d152b50c841fc69b562ccd0771ccd1c7c5ea9a0a2882f08ae48af3f8832fe0ff8d7c2b7244b8475e60b328e0a3908ce5f332d5ae69bc02bdc5541771dc28cd74c9ddcb5782f214fb2d1bd4711f280fa5108947a3ac4cabc4717fc7e65ceeaf2ebc21dfba18ca87a0daa213df42aba5ac3a85333535fc7628d93e1bbc90597ec0d3ff18a97bc120a166984025549aed24d1523c530e56d5580d718d8af14e366f7ecf63be51dc64c722ba37d2d6b5ff72a08420e19051ca4d5cd968b2b7ba6d31fa702c11d8220757f5cc912d1e9adff497a77c7c71486b5b00dd202efa90aadec7aa50c00b80a9f1fb1d20e3ae08381dd6139c867e4cfdf5e3eae1ed2806b6dd24f3688075b194060699a91554f46ca1061061fb13dca5fd582600837b7c4fb7206e331494bfac4fe2c277b9dde4713ce0f524b5e4219b4276ed92f16b24da902d4a49ab0f00f9fd732447b8823400497efb5806d999ab500045b438d0de1db4cd7703c618ccf31cc1f4b46436ef065dd80b97d351b4a740207554dbc181f9b5d09c66026f87d27bb031432098b5e50f46b99073f04ce47d8869a5af02461150d7c93eee03f0bd37e1a563150a7b9642e4886841cddc8152a6a6f7e0e61000eaa2e3c1d8c53c6548ceab3535a8d3441359f90a486f437285477a1c2608091e6756aa8bb33c3a4c24b3b8f9f5a076622b33bcd9e8e74aeed8d24fb66ceff0a353de335ad843499d040181d95ff0d4e650612a61ececac3055742822849d915803308439eef7098463a6d411497a504073f3d2210c247473a33fb23ebd52176b69160af1674683165852fe0265d");
    
    type shared_secret_array is array(0 to 1) of std_logic_vector(24*8-1 downto 0);
    constant shared_secret_kat : shared_secret_array :=(
    x"c4c011014b7a2161a8453ac46ccf76c07759615dcc28d8f8",
    x"c05fe6d102103e1a2c9f60210da49bb6824c5d38905ac170");
    
    type invalid_shared_secret_array is array(0 to 1) of std_logic_vector(24*8-1 downto 0); --For when decap ciphertext does not match
    constant invalid_shared_secret_kat : invalid_shared_secret_array :=(
    x"be32cc41e92a293194502b5515a175e243dc625280343ca7",
    x"0e52222158124028baee5cb93aa2a75e4166b130680c2395");    
    
    signal do_read1 : std_logic_vector(767 downto 0);
    signal do_read2 : std_logic_vector(767 downto 0);
    signal addr_part_1 : std_logic_vector(7 downto 0);
    signal w_one_8     : std_logic_vector(7 downto 0) := "00000001";
    signal w_addr_inc : std_logic_vector(2 downto 0);
    signal w_one_3      : std_logic_vector(2 downto 0) := "001";
    signal combine_key : std_logic_vector(767 downto 0);

begin

    -- 200 MHz clock
    oscillator : process
    begin
        clk <= '0';
        wait for PERIOD/2;
        clk <= '1';
        wait for PERIOD/2;
    end process oscillator;   

    uut : sike_p751
        port map (clk,rst,sike_cmd_i,iso_cmd_i,reg_sel_i,
                  wr_input_sel_i,wr_op_sel_i, wr_word_sel_i, wr_en_i,
                  rd_reg_i, rd_op_sel_i, rd_word_sel_i, buffer_xor_i,
                  keccak_clear_i, keccak_din_i, keccak_din_valid_byte_i, keccak_word_cnt_i, keccak_word_valid_i,
                  data_i, data_o, busy_o);
        
    -- Testbench
    tb : process
    variable outline: line;
    variable errors       : boolean := FALSE;
    file outfile: text is out "mem_out";
    begin
        
        wait for PERIOD;
        
        rst <= '1';
        
        wait for 5 * PERIOD;
        
        rst <= '0';               
            
        wait for PERIOD;

        -- Insert elements
        wr_en_i <= '1';
        wr_input_sel_i <= '0';
        wr_op_sel_i <= "10";
        
        wait for PERIOD;
        for j in 0 to 3 loop
            data_i <= secret_msg_alice_array_hex(0)(64*(j+1)-1 downto 64*j);
            wr_word_sel_i <= std_logic_vector(to_unsigned(j+8,4));
            wait for PERIOD;
        end loop;
        
        for j in 0 to 3 loop
            data_i <= secret_msg_bob_array_hex(0)(64*(j+1)-1 downto 64*j);
            wr_word_sel_i <= std_logic_vector(to_unsigned(j,4));
            wait for PERIOD;
        end loop;
        
        keccak_clear_i <= '1';
        wait for PERIOD;
        keccak_clear_i <= '0';
        
        wr_op_sel_i <= "00";
        
        addr_part_1 <= "00000000";
        w_addr_inc <= (others => '0');
        wait for PERIOD;
        for i in 0 to mont_array_hex'length-1 loop
            reg_sel_i <= addr_part_1;
            w_addr_inc <= (others => '0');
            wait for PERIOD;
            for j in 0 to 11 loop
                data_i <= mont_array_hex(i)(64*(j+1)-1 downto 64*j);
                wr_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD;
                w_addr_inc <= std_logic_vector(unsigned(w_addr_inc) + unsigned(w_one_3)); 
            end loop;
            addr_part_1 <= std_logic_vector(unsigned(addr_part_1)+unsigned(w_one_8));
            wait for PERIOD;
        end loop;
        wait for PERIOD;
        addr_part_1 <= std_logic_vector(unsigned(addr_part_1)+unsigned(w_one_8));
        wait for PERIOD;
        
        --addr_part_1 <= "00010000";
        w_addr_inc <= (others => '0');
        wait for PERIOD;
        for i in 0 to params_hex'length-1 loop
            
            reg_sel_i <= addr_part_1;
            w_addr_inc <= (others => '0');
            wait for PERIOD;
            for j in 0 to 11 loop
                data_i <= params_hex(i)(64*(j+1)-1 downto 64*j);
                wr_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD;
                w_addr_inc <= std_logic_vector(unsigned(w_addr_inc) + unsigned(w_one_3)); 
            end loop;
            addr_part_1 <= std_logic_vector(unsigned(addr_part_1)+unsigned(w_one_8));
            wait for PERIOD;
        end loop;
        
--        w_addr_inc <= (others => '0');
--        wait for PERIOD;
--        for i in 0 to round2_params_hex'length-1 loop
            
--            reg_sel_i <= addr_part_1;
--            w_addr_inc <= (others => '0');
--            wait for PERIOD;
--            for j in 0 to 11 loop
--                data_i <= round2_params_hex(i)(64*(j+1)-1 downto 64*j);
--                wr_word_sel_i <= std_logic_vector(to_unsigned(j,4));
--                wait for PERIOD;
--                w_addr_inc <= std_logic_vector(unsigned(w_addr_inc) + unsigned(w_one_3)); 
--            end loop;
--            addr_part_1 <= std_logic_vector(unsigned(addr_part_1)+unsigned(w_one_8));
--            wait for PERIOD;
--        end loop;
        
        --Shift in keys
        wr_op_sel_i <= "01";
        wr_en_i <= '1';
        
        combine_key <= "000000000000" & alice_keys_hex(0) & "00000" & bob_keys_hex(0);
        wait for 4*PERIOD;
        for j in 0 to 11 loop
            wr_word_sel_i <= std_logic_vector(to_unsigned(j,4));
            wait for PERIOD;
            data_i <= combine_key(64*(j+1)-1 downto 64*j);
            wait for PERIOD;
        end loop;
        wait for PERIOD;
        wr_en_i <= '0';
        wait for PERIOD;
        
        --#############################KEYGEN#################################
        sike_cmd_i <= "001";
        wait for PERIOD;
        sike_cmd_i <= "000";
        wait for PERIOD;
        while busy_o = '1' loop
            wait for PERIOD;
        end loop;       
        
        --Collect public key and secret key
        rd_op_sel_i <= "10";
        wait for PERIOD;
        for j in 0 to 3 loop
            rd_word_sel_i <= std_logic_vector(to_unsigned(j+8,4));
            wait for PERIOD;
            do_read2(64*(j+1)-1 downto 64*j) <= data_o;
            wait for PERIOD;
        end loop;
        secret_key(644*8-1 downto 612*8) <= do_read2(255 downto 0);
        wait for PERIOD;
        
        rd_op_sel_i <= "01";
        wait for PERIOD;
        for j in 0 to 5 loop
            rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
            wait for PERIOD;
            do_read2(64*(j+1)-1 downto 64*j) <= data_o;
            wait for PERIOD;
        end loop;
        secret_key(596*8-1 downto 0)     <= secret_key(644*8-1 downto 48*8);
        secret_key(644*8-1 downto 596*8) <= do_read2(48*8-1 downto 0);
        wait for PERIOD;        
        
        rd_op_sel_i <= "00";
        for i in 63 downto 62 loop
            reg_sel_i <= std_logic_vector(to_unsigned(i,8));
            rd_reg_i <= '1';
            wait for 5*PERIOD;
            for j in 0 to 11 loop
                rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD*5;
                do_read2(64*(j+1)-1 downto 64*j) <= data_o;
                wait for PERIOD;
            end loop;           
            wait for PERIOD;
            secret_key(550*8-1 downto 0)     <= secret_key(644*8-1 downto 94*8);
            public_key(470*8-1 downto 0)     <= public_key(564*8-1 downto 94*8);
            secret_key(644*8-1 downto 550*8) <= do_read2(751 downto 0);
            public_key(564*8-1 downto 470*8) <= do_read2(751 downto 0);
            wait for PERIOD;
        end loop;         

        for i in 67 downto 66 loop
            reg_sel_i <= std_logic_vector(to_unsigned(i,8));
            rd_reg_i <= '1';
            wait for 5*PERIOD;
            for j in 0 to 11 loop
                rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD*5;
                do_read2(64*(j+1)-1 downto 64*j) <= data_o;
                wait for PERIOD;
            end loop;           
            wait for PERIOD;
            secret_key(550*8-1 downto 0)     <= secret_key(644*8-1 downto 94*8);
            public_key(470*8-1 downto 0)     <= public_key(564*8-1 downto 94*8);
            secret_key(644*8-1 downto 550*8) <= do_read2(751 downto 0);
            public_key(564*8-1 downto 470*8) <= do_read2(751 downto 0);
            wait for PERIOD;
        end loop;   

        for i in 71 downto 70 loop
            reg_sel_i <= std_logic_vector(to_unsigned(i,8));
            rd_reg_i <= '1';
            wait for 5*PERIOD;
            for j in 0 to 11 loop
                rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD*5;
                do_read2(64*(j+1)-1 downto 64*j) <= data_o;
                wait for PERIOD;
            end loop;           
            wait for PERIOD;
            secret_key(550*8-1 downto 0)     <= secret_key(644*8-1 downto 94*8);
            public_key(470*8-1 downto 0)     <= public_key(564*8-1 downto 94*8);
            secret_key(644*8-1 downto 550*8) <= do_read2(751 downto 0);
            public_key(564*8-1 downto 470*8) <= do_read2(751 downto 0);
            wait for PERIOD;
        end loop;  
        
        if (public_key /= public_key_kat(0)) then
            assert false
                report "Public keys do not match!";
            errors := TRUE;
        end if; 
 
        if (secret_key /= secret_key_kat(0)) then
            assert false
                report "Secret keys do not match!";
            errors := TRUE;
        end if; 
        
        --#############################ENCAPSULATION#################################
        sike_cmd_i <= "010";
        wait for PERIOD;
        sike_cmd_i <= "000";
        wait for PERIOD;
        while busy_o = '1' loop
            wait for PERIOD;
        end loop;
        
        --Collect ciphertext and shared_secret
        rd_op_sel_i <= "00";
        for i in 51 downto 50 loop
            reg_sel_i <= std_logic_vector(to_unsigned(i,8));
            rd_reg_i <= '1';
            wait for 5*PERIOD;
            for j in 0 to 11 loop
                rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD*5;
                do_read2(64*(j+1)-1 downto 64*j) <= data_o;
                wait for PERIOD;
            end loop;           
            wait for PERIOD;
            cipher_text(502*8-1 downto 0)    <= cipher_text(596*8-1 downto 94*8);
            cipher_text(596*8-1 downto 502*8)<= do_read2(751 downto 0);
            wait for PERIOD;
        end loop;         

        for i in 55 downto 54 loop
            reg_sel_i <= std_logic_vector(to_unsigned(i,8));
            rd_reg_i <= '1';
            wait for 5*PERIOD;
            for j in 0 to 11 loop
                rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD*5;
                do_read2(64*(j+1)-1 downto 64*j) <= data_o;
                wait for PERIOD;
            end loop;           
            wait for PERIOD;
            cipher_text(502*8-1 downto 0)    <= cipher_text(596*8-1 downto 94*8);
            cipher_text(596*8-1 downto 502*8)<= do_read2(751 downto 0);
            wait for PERIOD;
        end loop;   

        for i in 59 downto 58 loop
            reg_sel_i <= std_logic_vector(to_unsigned(i,8));
            rd_reg_i <= '1';
            wait for 5*PERIOD;
            for j in 0 to 11 loop
                rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD*5;
                do_read2(64*(j+1)-1 downto 64*j) <= data_o;
                wait for PERIOD;
            end loop;           
            wait for PERIOD;
            cipher_text(502*8-1 downto 0)    <= cipher_text(596*8-1 downto 94*8);
            cipher_text(596*8-1 downto 502*8)<= do_read2(751 downto 0);
            wait for PERIOD;
        end loop;       

        rd_op_sel_i <= "10";
        wait for PERIOD;
        for j in 0 to 3 loop
            rd_word_sel_i <= std_logic_vector(to_unsigned(j+4,4));
            wait for PERIOD;
            do_read2(64*(j+1)-1 downto 64*j) <= data_o;
            wait for PERIOD;
        end loop;
        cipher_text(564*8-1 downto 0)     <= cipher_text(596*8-1 downto 32*8);
        cipher_text(596*8-1 downto 564*8) <= do_read2(32*8-1 downto 0);
        wait for PERIOD;        
            
        rd_op_sel_i <= "11";
        wait for PERIOD;
        for j in 0 to 2 loop
            rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
            wait for PERIOD;
            do_read2(64*(j+1)-1 downto 64*j) <= data_o;
            wait for PERIOD;
        end loop;
        shared_secretA(24*8-1 downto 0)     <= do_read2(24*8-1 downto 0);
        wait for PERIOD;        

        if (cipher_text /= cipher_text_kat(0)) then
            assert false
                report "Cipher texts do not match!";
            errors := TRUE;
        end if; 

        if (shared_secretA /= shared_secret_kat(0)) then
            assert false
                report "Encap shared secrets do not match!";
            errors := TRUE;
        end if; 
       
        --Perform Decapsulation after swapping places of messages
        wr_op_sel_i <= "10";
        rd_op_sel_i <= "10";
        wr_input_sel_i <= '1';
        for j in 0 to 3 loop
            rd_word_sel_i <= std_logic_vector(to_unsigned(j+4,4));
            wr_word_sel_i <= std_logic_vector(to_unsigned(j,4));
            wr_en_i <= '1';
            wait for PERIOD;
        end loop;        
        wr_en_i <= '0';
        wr_input_sel_i <= '0';
        
        wait for PERIOD;
        
        --#############################DECAPSULATION#################################
        sike_cmd_i <= "011";
        wait for PERIOD;
        sike_cmd_i <= "000";
        wait for PERIOD;
        while busy_o = '1' loop
            wait for PERIOD;
        end loop;        

        --sike_cmd_i <= "100"; For when ciphertexts do not match
        sike_cmd_i <= "101";
        wait for PERIOD;
        sike_cmd_i <= "000";
        wait for PERIOD;
        while busy_o = '1' loop
            wait for PERIOD;
        end loop;      
        
        --Collect shared secret
        rd_op_sel_i <= "11";
        wait for PERIOD;
        for j in 0 to 2 loop
            rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
            wait for PERIOD;
            do_read2(64*(j+1)-1 downto 64*j) <= data_o;
            wait for PERIOD;
        end loop;
        shared_secretB(24*8-1 downto 0)     <= do_read2(24*8-1 downto 0);
        wait for PERIOD;         

        if (shared_secretB /= shared_secret_kat(0)) then
            assert false
                report "Decap shared secrets do not match!";
            errors := TRUE;
        end if; 

        list_mem <= '1';
        wait for PERIOD*2;
        list_mem <= '0';
        assert false
            report "Listing Memory"
                severity note;
        wait for PERIOD*2;
        --addr_part_1 <= (others => '0');
        rd_op_sel_i <= "00";
        for i in 0 to 255 loop
            reg_sel_i <= std_logic_vector(to_unsigned(i,8));
            rd_reg_i <= '1';
            wait for 5*PERIOD;
            for j in 0 to 11 loop
                rd_word_sel_i <= std_logic_vector(to_unsigned(j,4));
                wait for PERIOD*5;
                do_read2(64*(j+1)-1 downto 64*j) <= data_o;
                wait for PERIOD;
            end loop;
            wait for PERIOD;
            hwrite(outline, reg_sel_i);
            write(outline, string'(""" = """));
            hwrite(outline,do_read2(767 downto 0));
            writeline(outfile,outline);
        end loop;
        write(outline, string'(""" pk = """));
        hwrite(outline,public_key);
        writeline(outfile,outline);
        wait for PERIOD;
        write(outline, string'(""" sk = """));
        hwrite(outline,secret_key);
        writeline(outfile,outline);
        wait for PERIOD;    
        write(outline, string'(""" ct = """));
        hwrite(outline,cipher_text);
        writeline(outfile,outline);
        wait for PERIOD;
        write(outline, string'(""" ssA = """));
        hwrite(outline,shared_secretA);
        writeline(outfile,outline);
        wait for PERIOD;  
        write(outline, string'(""" ssB = """));
        hwrite(outline,shared_secretB);
        writeline(outfile,outline);
        wait for PERIOD;                            
        list_mem_done <= '1';    
        
        assert false report "Simulation finished." severity note;
        
        assert errors
            report "KAT test vector passed."
                severity note;
        assert not errors
            report "KAT test vector failed."
                severity note;        
        wait;
        
    end process tb;        

end Behavioral;
