--********************************************************************************************
--* VHDL-SIKE: a speed optimized hardware implementation of the 
--*            Supersingular Isogeny Key Encapsulation scheme
--*
--*    Copyright (c) Brian Koziel and Reza Azarderakhsh
--*
--********************************************************************************************* 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity mux_generic is
    generic (
            SZ : integer := 16);
    port (  a  : in std_logic_vector(SZ-1 downto 0);
            b  : in std_logic_vector(SZ-1 downto 0);
            sel: in std_logic;
            c  : out std_logic_vector(SZ-1 downto 0));
end mux_generic;

architecture Behavioral of mux_generic is

begin
   
mux_proc : process(a, b, sel)
begin
case sel is
    when '0' =>
        c <= a;
    when '1' =>
        c <= b;
    when others =>
end case;
end process;
end Behavioral;
