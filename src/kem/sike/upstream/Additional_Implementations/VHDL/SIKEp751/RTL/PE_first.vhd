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

entity PE_first is
    port(clk     : in std_logic;
         rst     : in std_logic;
         si_i    : in std_logic_vector(15 downto 0);
         m_barj_i: in std_logic_vector(15 downto 0);
         c_o     : out std_logic_vector(16 downto 0));
end PE_first;

architecture Behavioral of PE_first is

COMPONENT mult_16_dsp
  PORT (
    A : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
    B : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
    P : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
  );
END COMPONENT;

signal s_in_exp   : std_logic_vector(31 downto 0);
signal mult_out : std_logic_vector(31 downto 0);
signal res      : std_logic_vector(31 downto 0);

begin

s_in_exp(31 downto 16) <= (others => '0');
s_in_exp(15 downto 0)  <= si_i;

MULT1 : mult_16_dsp
    port map(m_barj_i,si_i, mult_out);
 
res <= std_logic_vector(unsigned(mult_out) + unsigned(s_in_exp));

i_output_regs : process(clk, rst)
begin
if rst = '1' then
    c_o <= (others => '0');
elsif rising_edge(clk) then
    c_o(15 downto 0) <= res(31 downto 16);
end if;
end process i_output_regs;

c_o(16) <= '0';

end Behavioral;
