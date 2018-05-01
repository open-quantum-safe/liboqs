#VHDL-SIKE README
By Brian Koziel and Reza Azarderakhsh

--********************************************************************************************
--* VHDL-SIKE: a speed optimized hardware implementation of the 
--*            Supersingular Isogeny Key Encapsulation scheme
--*
--*    Copyright (c) Brian Koziel and Reza Azarderakhsh
--*
--********************************************************************************************* 

VHDL-SIKE is a VHDL implementation of the supersingular isogeny
Diffie-Hellman Key Encapsulation scheme. The public parameters
(SIKEp751) chosen in this implementation provide approximately 186 bits
of classical security and 124 bits of quantum security. This
implementation was made as a proof of concept to see how fast SIKE could
be pushed if implemented in hardware.

1. Contents:

/RTL/
    add_sub_752.vhd
    add_sub_gen.vhd
    fau_752.vhd
	iso_ctrl.vhd
	keccak_1088.vhd
	keccak_buffer_1088.vhd
	keccak_globals.vhd
	keccak_round.vhd
	keccak_round_constants_gen.vhd
    mm_752.vhd
    mult_unit_752.vhd
    mux_generic.vhd
    PE.vhd
    PE_final.vhd
    PE_first.vhd
    PE_prefinal.vhd
	sike_arith_unit.vhd
	sike_p751.vhd
/TB/
    sike_p751_tb.vhd
constraints.xdc
README.txt
RAMLayout.txt
lut43_p751.coe
sidhROM_p751.coe
sikeROM_p751.coe

2. Main Features:

-Speed optimized hardware implementation of SIKE with 124 bits of
quantum security and 186 bits of classical security
-Constant-time implementation with projective Montgomery isogeny
formulas
-Includes RTL, program ROM, and a simple testbench

3. Implementation Options:

VHDL-SIKE was built for FPGA using Xilinx Vivado 2015.4. Vivado's IP
tools were made to set up a register file, isogeny program ROM file,
SIKE program ROM file, strategy lookup ROM file, and 16x16DSP
multiplier. The files can also be used to synthesize with ASIC cells.
For a full estimate of hardware resources, one can include the
definitions for the register file, program ROM file, and strategy lookup
ROM file. Targeted part is a Xilinx Virtex-7 xc7vx690tffg1157-3.

3.1 IP Generation

Using Xilinx Vivado's IP Generator, generate the following IP with the
following options:
	lut43_table: Block Memory Generator
	prog_rom: Block Memory Generator
	sike_rom: Block Memory Generator
	reg_file_752: Block Memory Generator
	mult_16_dsp: Multiplier

3.1.1 lut43_table
IP: Block Memory Generator
Basic
  Interface type: Native
  Memory type: Single Port ROM
Port A Options
  Port A Width: 9
  Port A Depth: 2048 bits
  Primitives Output Registered
Other Options
  Load init file --> load lut43_p751.coe

3.1.2 prog_rom
IP: Block Memory Generator
Basic
  Interface type: Native
  Memory type: Single Port ROM
Port A Options
  Port A Width: 26
  Port A Depth: 45000
  Primitives Output Registered
Other Options
  Load init file --> load sidhROM_p751.coe

3.1.3 sike_rom
IP: Block Memory Generator
Basic
  Interface type: Native
  Memory type: Single Port ROM
Port A Options
  Port A Width: 32
  Port A Depth: 2048
  Primitives Output Registered
Other Options
  Load init file --> load sikeROM_p751.coe
  
3.1.4 reg_file_752
IP: Block Memory Generator
Basic
  Interface type: Native
  Memory type: True Dual Port RAM 
Port A Options
  Port A Width: 752
  Port A Depth: 256
  Primitives Output Registered
  Operating Mode: Write First
  Enable Port Type: Use ENA Pin
Port B Options
  (Same as Port A)

3.1.5 mult_16_dsp
IP: Multiplier
Basic
  Multiplier type: Parallel Multiplier
  A: Unsigned 16 bit
  B: Unsigned 16 bit
  Multiplier Construction: Use Mults
  Optimization Options: Speed Optimized
Output and Control
  Pipeline Stages: 0

4 Running the Testbench

Upon creating the IP as described above with the correct sets of ROM,
the VHDL code can be tested and verified with the testbench. This
testbench initializes the register file RAM to contain the public
parameters listed for p751 as well as the key and secret message buffer.
The values used in the initial testbench correspond to KAT0 for
SIKEp751. There are a few important values for Montgomery multiplication
that appear at the beginning of the register file.  All arithmetic is in
the Montgomery domain and converted back to the regular domain at the
end. 

Since large-degree isogenies take a long time to simulate, one single
test case is generally performed per simulation. One can choose use
different KATs for keys and messages and verify that the final answer is
correct. Both Alice (iso2) and Bob (iso3) expect a key of full length. 
This is a key of length 372 bits for Alice and 379 bits for Bob.  On an
i7-4790k processor, a full simulation of SIKE with 4 replicated
multipliers takes around 6 hours.

For the provided testbench, upon finishing the test vector, the
simulation will automatically print the register file contents and KAT
values. Note that we list the values from MSB to LSB. The bytes are
aligned differently from in the software KAT (reverse order). The correct
values for SIKE with KAT0 are as follows:

pk = 576C172398783603445B3727ABC64AC0B703D889F891630C3B77B3FDA0AE6FC00D28DA23BACF79A7DC877CFB1C5D239D70260633941C49609B35B208B99800A96B34F3EA839F3AED7B24BFFE558117DD9889D7D491037D97A625BDB65B0145338140269DDD3C2ED2BB2EB75AE3F15FE5CFA6A173A8274963C4FE311DBEDE26D5677FE6D42F778C21F3367A215FD1B252DD99C3C9F509A8E38C0F558735748FBD5CEBA1B4BB9955F3812B8F60E6A38F6D6319A88112714A97C5BE4E2D37B604B67AA8212C47E7C2717B9B5CA700F96930B6AFA739A975E9C7B46B5DF8EC90B2B7808E84EFC8872C9FAA290FE001B9D7C9DA5D041D1B1A9070D99D0BE1935F4609FCC2AA9F42F71F3290C78A16E1CAC429CC9B807A1D689A34C5755CA5AED402E06D2154F2132D2D21B63E26FAA24D5444CC82B318202B9C4A3D3D8C34239DB1782D06C1F3C2154A1114D96A1946A81D91B737A5F30C7F3918F9907147550B4EF7AFF08C902C8AE7D96F6F060BAFBE89DAA0764C201805A75E21841470CCEFBBC84C211CD041E8FC0102F545A0AF3F0875654DE98A8D9994CCE28C32D2F0AD1A08C310F1BED99136DE365103CF9C2A234889FECB0854D7CFBA04A0907F0D312055EC4272F99FFF11F1E50C85F04DCA1DC876BED7C3F2FA35EA393834A77E05064990B64EE70CBA36C16E8BDBDC9B781B750C421B127E0A61DD84E620A549BF065FF049054C90FFB58CD62012880CEF5284B4BBBEA451CC951A14B09FF34DB7A715D00A9657493A36B4301B1AA1B904556668E2557C
sk = 576C172398783603445B3727ABC64AC0B703D889F891630C3B77B3FDA0AE6FC00D28DA23BACF79A7DC877CFB1C5D239D70260633941C49609B35B208B99800A96B34F3EA839F3AED7B24BFFE558117DD9889D7D491037D97A625BDB65B0145338140269DDD3C2ED2BB2EB75AE3F15FE5CFA6A173A8274963C4FE311DBEDE26D5677FE6D42F778C21F3367A215FD1B252DD99C3C9F509A8E38C0F558735748FBD5CEBA1B4BB9955F3812B8F60E6A38F6D6319A88112714A97C5BE4E2D37B604B67AA8212C47E7C2717B9B5CA700F96930B6AFA739A975E9C7B46B5DF8EC90B2B7808E84EFC8872C9FAA290FE001B9D7C9DA5D041D1B1A9070D99D0BE1935F4609FCC2AA9F42F71F3290C78A16E1CAC429CC9B807A1D689A34C5755CA5AED402E06D2154F2132D2D21B63E26FAA24D5444CC82B318202B9C4A3D3D8C34239DB1782D06C1F3C2154A1114D96A1946A81D91B737A5F30C7F3918F9907147550B4EF7AFF08C902C8AE7D96F6F060BAFBE89DAA0764C201805A75E21841470CCEFBBC84C211CD041E8FC0102F545A0AF3F0875654DE98A8D9994CCE28C32D2F0AD1A08C310F1BED99136DE365103CF9C2A234889FECB0854D7CFBA04A0907F0D312055EC4272F99FFF11F1E50C85F04DCA1DC876BED7C3F2FA35EA393834A77E05064990B64EE70CBA36C16E8BDBDC9B781B750C421B127E0A61DD84E620A549BF065FF049054C90FFB58CD62012880CEF5284B4BBBEA451CC951A14B09FF34DB7A715D00A9657493A36B4301B1AA1B904556668E2557C0102505C57D33805E406218589720AAB8FFB2B878B9EFA90DC137D406760550E21F856B9593BE000081451D479ED26862D7F73369973CD2D0348B1CC251AD82FDD1A6BDBE4106D0CAA9476B0A035997C
ct = A026C563918DEA6323F6F62FCCA20E6D3EF1DDC408C900D7A193E985E64EDF5B6BED91B243AAF49D3454C1211CB3F9F392E4F226DE20420D73E03787F32FF57D180DCF67722E00D7E5B89B9BBD5297216FE136458060F55382EB5598D2F364BE3539E2545868C8BE5D0C7E29C88C366F7CB59713ACFECEEE4B144244B8B9012F6FCBAE1F1BFA45101D68A067AAC957054E649B2632CFE7E5FBA95A6C30D137DDCE8146685A5DC54A2A20D742CD30E2295ECF2054B6C70AE53B8E8C490F766F56FD7CC59F1DEA4FB36C571419C1DB81E89C4F0C6D18BEC76F2292BABD3F853C360A21D0FBEE894C32274ED17C72F8780E051FB1ED54C71D64FD9F1133366BB0C68C85FC666FDA1B938C765A199D39DD0D3A973025EADFCBC330C22E4BFFC92E9FCD9126A9F446AE846F37065B37E8D4AE6119740D8E83BF70261237BB10DDC7E5262EEFD9ED52649C7AFED747491AF7ACF5E879795E01337CC797F21A60AE5B4384C4E2A4204E406BD8134BDA30496461564A1FD3323DEA48E1FADC523B6DF5D7FCCE5EAEF5DECEA32E6DEFF883516D6FF3085CFC947F2EB549939A937CC243E46CD5D6E1B4EFB74279E368F2506C84D02C8838734044EA43E850B1ED9725DC0989BB907BC8B0A94FC861A433B08D89ACB4B5A80970815DF52EC63245E76C125E17C196A78EA335C7705AA6EE52243EA249F021E70BC13D7FB9B77735136F1D03EDD31804226B7B20ADB13574983368FA6EB82FEDA2C828072124B2AC1DADB15133905EC522F5824B8C8C79986B67F5B617F6C588B6F3DB853AE2ED5B4E306F7EBBC6327210164A5AFFCFC3D53368B0F336253036
ssA = C4C011014B7A2161A8453AC46CCF76C07759615DCC28D8F8
ssB = C4C011014B7A2161A8453AC46CCF76C07759615DCC28D8F8
