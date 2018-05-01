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

entity add_sub_752 is
    port (  clk    : in std_logic;
            rst    : in std_logic;
            opa_i  : in std_logic_vector(751 downto 0);
            opb_i  : in std_logic_vector(751 downto 0);
            sub_i  : in std_logic;
            res_o  : out std_logic_vector(751 downto 0);
            neg_o  : out std_logic;
            zero_o : out std_logic);
end add_sub_752;

architecture Behavioral of add_sub_752 is

constant BASE_SZ : integer := 252;
constant NUM_ADDS : integer :=3;

component add_sub_gen is
    generic(
        WIDTH       : integer := 32);
    port (
        a_i           : in std_logic_vector(WIDTH-1 downto 0);
        b_i           : in std_logic_vector(WIDTH-1 downto 0);
        sub_i         : in std_logic; -- 0 = add, 1 = sub
        c_i           : in std_logic;
        res_o         : out std_logic_vector(WIDTH-1 downto 0);
        c_o           : out std_logic);
end component;

subtype add_word is std_logic_vector(BASE_SZ-1 downto 0);
type input_arr is array(0 to NUM_ADDS-1,0 to NUM_ADDS-1) of add_word;
type res_arr is array(0 to NUM_ADDS-1) of add_word;
type carry_arr is array(0 to NUM_ADDS) of std_logic;
type add_ctrl_arr is array(0 to NUM_ADDS-1) of std_logic;

signal a_reg_array : input_arr;
signal b_reg_array : input_arr;
signal res_array : res_arr;
signal res_reg_array : input_arr;
signal c_array : res_arr;
signal carry_array : carry_arr;
signal carry_reg_array : carry_arr;
signal add_ctrl_array : add_ctrl_arr;
signal add_ctrl_reg_array : add_ctrl_arr;

signal full_a     : std_logic_vector(BASE_SZ*NUM_ADDS-1 downto 0);
signal full_b     : std_logic_vector(BASE_SZ*NUM_ADDS-1 downto 0);
signal actual_res : std_logic_vector(BASE_SZ*NUM_ADDS-1 downto 0);

begin

full_a(751 downto 0) <= opa_i;
full_b(751 downto 0) <= opb_i;
full_a(755 downto 752) <= (others => '0');
full_b(755 downto 752) <= (others => '0');

GEN_ADD_SUB : for I in 0 to NUM_ADDS-1 generate
    
    FIRST_ELEMENT: if I = 0 generate
        ADD_SUB_GENERIC_INST_first : add_sub_gen
            generic map(BASE_SZ)
            port map(a_i   => full_a(BASE_SZ-1 downto 0),
	             b_i   => full_b(BASE_SZ-1 downto 0),
		     sub_i => sub_i,
		     c_i   => sub_i,
		     res_o => res_array(0),
		     c_o   => carry_array(1));    
    end generate FIRST_ELEMENT;
    OTHER_ELEMENTS: if I > 0 generate
        ADD_SUB_GENERIC_INST : add_sub_gen
            generic map(BASE_SZ)
            port map(a_i   => a_reg_array(I-1,I),
	             b_i   => b_reg_array(I-1,I),
		     sub_i => add_ctrl_reg_array(I-1),
		     c_i   => carry_reg_array(I),
		     res_o => res_array(I),
		     c_o   => carry_array(I+1));
    end generate OTHER_ELEMENTS;
end generate GEN_ADD_SUB;


temp_reg : process(clk, rst)
begin
if rst = '1' then
    
    carry_reg_array(NUM_ADDS) <= '0';
    add_ctrl_reg_array(0) <= '0';
    for j in 1 to NUM_ADDS-1 loop --Initialize a and b
        a_reg_array(0,j) <= (others => '0');
        b_reg_array(0,j) <= (others => '0');
    end loop;
    res_reg_array(0,0) <= (others => '0');
    for i in 1 to NUM_ADDS-1 loop
        carry_reg_array(i) <= '0';
        add_ctrl_reg_array(i) <= '0';
        for j in 0 to NUM_ADDS-1 loop
            if (i >= j) then
                res_reg_array(i,j) <= (others => '0');
            end if;
            if (i < j) then
                a_reg_array(i,j) <= (others => '0');
                b_reg_array(i,j) <= (others => '0');            
            end if;
        end loop;
    end loop;
elsif rising_edge(clk) then
    carry_reg_array(NUM_ADDS) <= carry_array(NUM_ADDS);
    add_ctrl_reg_array(0) <= sub_i;
    for j in 1 to NUM_ADDS-1 loop --Initialize a and b
        a_reg_array(0,j) <= full_a((j+1)*(BASE_SZ)-1 downto (j)*(BASE_SZ));
        b_reg_array(0,j) <= full_b((j+1)*(BASE_SZ)-1 downto (j)*(BASE_SZ));
    end loop;
    res_reg_array(0,0) <= res_array(0);
    for i in 1 to NUM_ADDS-1 loop
        carry_reg_array(i) <= carry_array(i);
        add_ctrl_reg_array(i) <= add_ctrl_reg_array(i-1);
        for j in 0 to NUM_ADDS-1 loop
            
            if (i = j) then
                res_reg_array(i,j) <= res_array(i);
            elsif (i > j) then
                res_reg_array(i,j) <= res_reg_array(i-1,j);
            end if;
            if (i < j) then
                a_reg_array(i,j) <= a_reg_array(i-1,j);
                b_reg_array(i,j) <= b_reg_array(i-1,j);            
            end if;
        end loop;
    end loop;
    
end if;
end process;

combine_res : process(res_reg_array)
begin
for j in 0 to NUM_ADDS-1 loop
    actual_res((j+1)*(BASE_SZ)-1 downto (j)*(BASE_SZ)) <= res_reg_array(NUM_ADDS-1,j);
end loop;
end process;

res_o <= actual_res(751 downto 0);
neg_o <= actual_res(755);
zero_test : process(actual_res)
begin
if unsigned(actual_res) = 0 then
    zero_o <= '1';
else
    zero_o <= '0';
end if;
end process;

end Behavioral;
