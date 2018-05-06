--********************************************************************************************
--* VHDL-SIKE: a speed optimized hardware implementation of the 
--*            Supersingular Isogeny Key Encapsulation scheme
--*
--*    Copyright (c) Brian Koziel and Reza Azarderakhsh
--*
--********************************************************************************************* 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity mm_752 is
    port (clk      : in std_logic;
          rst      : in std_logic;
          start1_i   : in std_logic;
          start2_i   : in std_logic;
          opa1_i       : in std_logic_vector(751 downto 0);
          opa2_i       : in std_logic_vector(751 downto 0);
          opb1_i       : in std_logic_vector(751 downto 0);
          opb2_i       : in std_logic_vector(751 downto 0);
          even_odd_i : in std_logic; --Indicates 
          p1_o   : out std_logic_vector(751 downto 0);
          p2_o   : out std_logic_vector(751 downto 0));
end mm_752;

architecture Behavioral of mm_752 is

component PE_first is
    port(clk      : in std_logic;
         rst      : in std_logic;
         si_i     : in std_logic_vector(15 downto 0);
         m_barj_i : in std_logic_vector(15 downto 0);
         c_o      : out std_logic_vector(16 downto 0));
end component PE_first;

component PE is
    port(  clk      : in std_logic;
           rst      : in std_logic;
           qi_i     : in std_logic_vector(15 downto 0);
           ai_i     : in std_logic_vector(15 downto 0);
           bj_i     : in std_logic_vector(15 downto 0);
           m_barj_i : in std_logic_vector(15 downto 0);
           si_i     : in std_logic_vector(15 downto 0);
           ci_i     : in std_logic_vector(16 downto 0); --k+1 bits
           rst_o    : out std_logic;
           qip1_o   : out std_logic_vector(15 downto 0);
           cip1_o   : out std_logic_vector(16 downto 0); --k+1 bits
           sip1_o   : out std_logic_vector(15 downto 0);
           aip1_o   : out std_logic_vector(15 downto 0));
end component PE;

component PE_prefinal is
    port(  clk     : in std_logic;
           rst     : in std_logic;
           qi_i    : in std_logic_vector(15 downto 0);
           ai_i    : in std_logic_vector(15 downto 0);
           bj_i    : in std_logic_vector(15 downto 0);
           si_i    : in std_logic_vector(15 downto 0);
           ci_i    : in std_logic_vector(16 downto 0); --k+1 bits
           rst_o   : out std_logic;
           qip1_o  : out std_logic_vector(15 downto 0);
           cip1_o  : out std_logic_vector(16 downto 0); --k+1 bits
           sip1_o  : out std_logic_vector(15 downto 0);
           aip1_o  : out std_logic_vector(15 downto 0));
end component PE_prefinal;

component PE_final is
    port(  clk    : in std_logic;
           rst    : in std_logic;
           si_i   : in std_logic_vector(1 downto 0);
           ci_i   : in std_logic_vector(16 downto 0); --k+1 bits
           sip1_o : out std_logic_vector(15 downto 0);
           cip1_o : out std_logic_vector(1 downto 0)); --k+1 bits      
end component PE_final;

component mux_generic is
    generic (
        SZ    : integer := 16);
    port(  a  : in std_logic_vector(SZ-1 downto 0);
        b     : in std_logic_vector(SZ-1 downto 0);
        sel   : in std_logic;
        c     : out std_logic_vector(SZ-1 downto 0));
end component mux_generic;

type carry_arr is array(0 to 47) of std_logic_vector(16 downto 0);
type s_arr     is array(0 to 48) of std_logic_vector(15 downto 0);
type q_arr     is array(0 to 47) of std_logic_vector(15 downto 0);
type a_arr     is array(0 to 47) of std_logic_vector(15 downto 0);
type rst_arr   is array(0 to 47) of std_logic;

signal sel_array1  : std_logic_vector(151 downto 0); --3m+7 cycles
signal sel_array2  : std_logic_vector(151 downto 0);
signal carry_array : carry_arr;
signal s_array     : s_arr;
signal q_array     : q_arr;
signal a_array     : a_arr;
signal rst_array   : rst_arr; 

signal rst_pump1  : std_logic;
signal rst_pump2  : std_logic;
signal m_bar       : std_logic_vector(751 downto 0);

signal q_hold      : std_logic_vector(15 downto 0); --Hold s0 for 1 cycle
signal a_reg1      : std_logic_vector(751 downto 0);
signal a_reg2      : std_logic_vector(751 downto 0);
signal a_mux       : std_logic_vector(15 downto 0);
signal p_reg1      : std_logic_vector(751 downto 0);
signal p_reg2      : std_logic_vector(751 downto 0);
signal p_mux_out1  : std_logic_vector(751 downto 0);
signal p_mux_out2  : std_logic_vector(751 downto 0);

signal b_mux       : std_logic_vector(751 downto 0);
signal b_in1       : std_logic_vector(751 downto 0);
signal b_in2       : std_logic_vector(751 downto 0);
signal b_reg1      : std_logic_vector(751 downto 0);
signal b_reg2      : std_logic_vector(751 downto 0);

signal mult_in_reg_a : std_logic_vector(15 downto 0);
signal mult_in_reg_b : std_logic_vector(751 downto 0);
signal rst_pump_mux: std_logic;

begin

--m_bar is simply the modulus for SIDH primes with 2^ea * lb^eb * f -1
m_bar <= x"6fe5d541f71c0e12909f97badc668562b5045cb25748084e9867d6ebe876da959b1a13f7cc76e3ec968549f878a8eeafffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
q_array(0) <= q_hold;
rst_array(0) <= rst_pump_mux;
--Final sum is simply a 2 bit carry
s_array(48)(15 downto 2) <= (others => '0');
a_array(0) <= mult_in_reg_a;

FIRST_BOX : PE_first
    port map(clk      => clk, 
             rst      => rst, 
	     si_i     => s_array(0), 
	     m_barj_i => m_bar(15 downto 0),
	     c_o      => carry_array(0));
    
GEN_BOXES : for I in 0 to 45 generate --m+1 general boxes
    BOX_I : PE
        port map(clk      => clk, 
	         rst      => rst_array(I), 
		 qi_i     => q_array(I), 
		 ai_i     => a_array(I), 
                 bj_i     => mult_in_reg_b((I+1)*16-1 downto I*16), 
		 m_barj_i => m_bar((I+2)*16-1 downto (I+1)*16),
                 si_i     => s_array(I+1), 
		 ci_i     => carry_array(I), 
                 rst_o    => rst_array(I+1), 
		 qip1_o   => q_array(I+1), 
		 cip1_o   => carry_array(I+1),
                 sip1_o   => s_array(I), 
		 aip1_o   => a_array(I+1));
end generate;

SECOND_LAST_BOX : PE_prefinal --Box 46, m_bar is zero
    port map(clk    => clk, 
             rst    => rst_array(46), 
	     qi_i   => q_array(46), 
	     ai_i   => a_array(46), 
	     bj_i   => mult_in_reg_b(751 downto 736),
             si_i   => s_array(47), 
	     ci_i   => carry_array(46), 
	     rst_o  => rst_array(47),
             qip1_o => q_array(47), 
	     cip1_o => carry_array(47), 
	     sip1_o => s_array(46), 
	     aip1_o => a_array(47));

LAST_BOX : PE_final --Box 47, no outputs. m_bar and bi are zero
    port map(clk =>    clk, 
             rst =>    rst_array(47), 
	     si_i =>   s_array(48)(1 downto 0), 
	     ci_i =>   carry_array(47),
             sip1_o => s_array(47), 
	     cip1_o => s_array(48)(1 downto 0));

GEN_P_MUX:
for I in 0 to 46 generate
    P_MUX_16_1 : mux_generic --first output is ready after 2m+6 cycles
        generic map(16)
        port map(a   => p_reg1((I+1)*16 -1 downto I*16),
	         b   => s_array(I),
		 sel => sel_array1(97+I),
		 c   => p_mux_out1((I+1)*16 -1 downto I*16));
    P_MUX_16_2 : mux_generic --first output is ready after 2m+6 cycles
        generic map(16)
        port map(a   => p_reg2((I+1)*16 -1 downto I*16),
	         b   => s_array(I),
		 sel => sel_array2(97+I),
		 c   => p_mux_out2((I+1)*16 -1 downto I*16));
end generate;

A_MUX_16 : mux_generic --Determines which a is pushed through the system
    generic map(16)
    port map(a   => a_reg2(15 downto 0), 
             b   => a_reg1(15 downto 0),  
	     sel => even_odd_i, 
	     c   => a_mux);
    
MULT_INPUT_REGS : process(rst, clk)
begin
if rst = '1' then
    mult_in_reg_a <= (others => '0');
    mult_in_reg_b <= (others => '0');
elsif rising_edge(clk) then
    mult_in_reg_a <= a_mux;
    mult_in_reg_b <= b_mux;
end if;
end process;
    

PUMP_MUX : process(rst_pump1, rst_pump2,even_odd_i)
begin
case even_odd_i is
    when '0' =>
        rst_pump_mux <= rst_pump1;
    when '1' =>
        rst_pump_mux <= rst_pump2;
    when others =>
end case;
end process;

GEN_B_IN_MUX:
for I in 0 to 46 generate
    B_IN_MUX_16_1 : mux_generic
        generic map(16)
        port map(a   => b_reg1((I+1)*16-1 downto I*16), 
	         b   => opb1_i((I+1)*16 -1 downto I*16),
		 sel => sel_array1(I),
		 c   => b_in1((I+1)*16-1 downto I*16));
    B_IN_MUX_16_2 : mux_generic
        generic map(16)
        port map(a   => b_reg2((I+1)*16-1 downto I*16), 
	         b   => opb2_i((I+1)*16 -1 downto I*16),
		 sel => sel_array2(I),
		 c   => b_in2((I+1)*16-1 downto I*16));
end generate;

B_REG : process(clk, rst)
begin
if rst = '1' then
    b_reg1 <= (others => '0');
    b_reg2 <= (others => '0');
elsif rising_edge(clk) then
    b_reg1 <= b_in1;
    b_reg2 <= b_in2;
end if;
end process;

GEN_B_MUX:
for I in 0 to 46 generate
    i_even: if (I mod 2 = 0) generate
        B_MUX_16_1 : mux_generic
            generic map(16)
            port map(a   => b_reg2((I+1)*16 -1 downto I*16),
	             b   => b_reg1((I+1)*16 -1 downto I*16), 
		     sel => even_odd_i, 
		     c   => b_mux((I+1)*16-1 downto I*16));
    end generate i_even;
    
    i_odd: if (I mod 2 = 1) generate
        B_MUX_16_2 : mux_generic
            generic map(16)
            port map(a   => b_reg1((I+1)*16 -1 downto I*16), 
	             b   => b_reg2((I+1)*16 -1 downto I*16), 
		     sel => even_odd_i, 
		     c   => b_mux((I+1)*16-1 downto I*16));
    end generate i_odd;
end generate;

input_regs: process(clk, rst)
begin
if rst = '1' then
    a_reg1 <= (others => '0');
    a_reg2 <= (others => '0');
elsif rising_edge(clk) then
    if start1_i = '1' then
        a_reg1 <= opa1_i;
    elsif even_odd_i = '1' then
        a_reg1(735 downto 0)   <= a_reg1(751 downto 16);
        a_reg1(751 downto 736) <= (others => '0'); --Shift 
    else
        a_reg1 <= a_reg1;
    end if;
    
    
    if start2_i = '1' then
        a_reg2 <= opa2_i;
    elsif even_odd_i = '0' then
        a_reg2(735 downto 0)   <= a_reg2(751 downto 16);
        a_reg2(751 downto 736) <= (others => '0');
    else
        a_reg2 <= a_reg2;
    end if;
end if;
end process;

--Simply shifts bits until they signal the output to capture
counter_regs: process(clk, rst)
begin
if rst = '1' then
    sel_array1 <= (others => '0');
    sel_array2 <= (others => '0');
    rst_pump1  <= '1';
    rst_pump2  <= '1';
    q_hold     <= (others => '0');
elsif rising_edge(clk) then
    sel_array1(151 downto 1) <= sel_array1(150 downto 0);
    sel_array2(151 downto 1) <= sel_array2(150 downto 0);
    q_hold <= s_array(0);
    if start1_i = '1' then
        sel_array1(0) <= '1';
        rst_pump1     <= '1';
    else
        sel_array1(0) <= '0';
        rst_pump1     <= '0';
    end if;
    
    if start2_i = '1' then
        sel_array2(0) <= '1';
        rst_pump2     <= '1';
    else
        sel_array2(0) <= '0';
        rst_pump2     <= '0';
    end if;
end if;
end process;

i_output_regs: process(clk, rst)
begin
if rst = '1' then
    p_reg1 <= (others => '0');
    p_reg2 <= (others => '0');
elsif rising_edge(clk) then
    p_reg1 <= p_mux_out1;
    p_reg2 <= p_mux_out2;    
end if;
end process;

p1_o <= p_reg1;
p2_o <= p_reg2;

end Behavioral;
