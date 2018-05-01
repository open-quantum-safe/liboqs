--********************************************************************************************
--* VHDL-SIKE: a speed optimized hardware implementation of the 
--*            Supersingular Isogeny Key Encapsulation scheme
--*
--*    Copyright (c) Brian Koziel and Reza Azarderakhsh
--*
--********************************************************************************************* 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity PE_prefinal is
    port(  clk    : in std_logic;
           rst    : in std_logic;
           qi_i   : in std_logic_vector(15 downto 0);
           ai_i   : in std_logic_vector(15 downto 0);
           bj_i   : in std_logic_vector(15 downto 0);
           si_i   : in std_logic_vector(15 downto 0);
           ci_i   : in std_logic_vector(16 downto 0); --k+1 bits
           rst_o  : out std_logic;
           qip1_o : out std_logic_vector(15 downto 0);
           cip1_o : out std_logic_vector(16 downto 0); --k+1 bits
           sip1_o : out std_logic_vector(15 downto 0);
           aip1_o : out std_logic_vector(15 downto 0));
           
end PE_prefinal;

architecture Behavioral of PE_prefinal is

COMPONENT mult_16_dsp
  PORT (
    A : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
    B : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
    P : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
  );
END COMPONENT;

signal s_in_exp : std_logic_vector(32 downto 0);
signal c_in_exp : std_logic_vector(32 downto 0);
signal mult_res : std_logic_vector(31 downto 0);
signal res : std_logic_vector(32 downto 0); --33 bits

begin
    
s_in_exp(32 downto 16) <= (others => '0');
s_in_exp(15 downto 0)  <= si_i;
c_in_exp(32 downto 17) <= (others => '0');
c_in_exp(16 downto 0)  <= ci_i;

MULT : mult_16_dsp
    port map(ai_i, bj_i, mult_res);
    
res <= std_logic_vector(unsigned('0' & mult_res) + unsigned(s_in_exp) + unsigned(c_in_exp));

i_output_regs : process(clk) --synchronous reset
begin
if rising_edge(clk) then
    rst_o  <= rst;
    qip1_o <= qi_i;
    aip1_o <= ai_i;
    if rst = '1' then
        cip1_o <= (others => '0');
        sip1_o <= (others => '0');
    else
        cip1_o <= res(32 downto 16);
        sip1_o <= res(15 downto 0);
    end if;
end if;
end process;

end Behavioral;
