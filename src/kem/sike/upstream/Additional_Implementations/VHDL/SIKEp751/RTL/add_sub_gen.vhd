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

--ADD SUB GENERIC
entity add_sub_gen is
    generic(
        WIDTH       : integer := 32);
    port (
        a_i           : in std_logic_vector(WIDTH-1 downto 0);
        b_i           : in std_logic_vector(WIDTH-1 downto 0);
        sub_i         : in std_logic; -- 0 = add, 1 = sub
        c_i           : in std_logic;
        res_o         : out std_logic_vector(WIDTH-1 downto 0);
        c_o           : out std_logic);
end add_sub_gen;

architecture Behavioral of add_sub_gen is

    signal c_in_exp : std_logic_vector(WIDTH downto 0);
    signal op2 : std_logic_vector(WIDTH-1 downto 0); --b for add or ~b for sub
    signal full_res : std_logic_vector(WIDTH downto 0); --W+1 bits for carry out

begin
    c_in_exp(WIDTH downto 1) <= (others => '0');
    c_in_exp(0) <= c_i;
    op2 <= b_i when sub_i = '0' else not b_i;
    
    full_res <= std_logic_vector(unsigned('0' & a_i) + unsigned('0' & op2) + unsigned(c_in_exp));
    
    res_o <= full_res(WIDTH-1 downto 0);
    c_o   <= full_res(WIDTH);



end Behavioral;
