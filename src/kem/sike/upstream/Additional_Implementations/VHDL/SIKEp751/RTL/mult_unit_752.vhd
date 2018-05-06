--********************************************************************************************
--* VHDL-SIKE: a speed optimized hardware implementation of the 
--*            Supersingular Isogeny Key Encapsulation scheme
--*
--*    Copyright (c) Brian Koziel and Reza Azarderakhsh
--*
--********************************************************************************************* 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity mult_unit_752 is
    generic (
        NUM_MULTS   : integer := 4
    );
    port (clk              : in std_logic;
          rst              : in std_logic;
          start_i          : in std_logic;
          opa_i            : in std_logic_vector(751 downto 0);
          opb_i            : in std_logic_vector(751 downto 0);
          res_read_i       : in std_logic;
          reset_even_odd_i : in std_logic;
          res_o            : out std_logic_vector(751 downto 0));          
end mult_unit_752;

architecture structural of mult_unit_752 is

component mux_generic is
    generic (
        SZ    : integer := 16);
    port(  a  : in std_logic_vector(SZ-1 downto 0);
        b     : in std_logic_vector(SZ-1 downto 0);
        sel   : in std_logic;
        c     : out std_logic_vector(SZ-1 downto 0));
end component mux_generic;

component mm_752 is
    port (clk        : in std_logic;
          rst        : in std_logic;
          start1_i   : in std_logic;
          start2_i   : in std_logic;
          opa1_i       : in std_logic_vector(751 downto 0);
          opa2_i       : in std_logic_vector(751 downto 0);
          opb1_i       : in std_logic_vector(751 downto 0);
          opb2_i       : in std_logic_vector(751 downto 0);
          even_odd_i : in std_logic;
          p1_o       : out std_logic_vector(751 downto 0);
          p2_o       : out std_logic_vector(751 downto 0));
end component mm_752;

type op_array_type is array (0 to NUM_MULTS*2-1) of std_logic_vector(751 downto 0);    
type res_array_type is array (0 to NUM_MULTS*2-1) of std_logic_vector(751 downto 0);

signal res_array : res_array_type;
signal opa_array : op_array_type;
signal opb_array : op_array_type;
signal a_next    : op_array_type;
signal b_next    : op_array_type;
signal start_array : std_logic_vector(NUM_MULTS*2-1 downto 0);
signal sixteen0 : std_logic_vector(15 downto 0);

signal current_mult_start : std_logic_vector(NUM_MULTS*2-1 downto 0) := (0 => '1', others => '0');
signal current_mult_rd : integer range 0 to NUM_MULTS*2-1 := 0;
signal even_odd : std_logic;

begin
sixteen0 <= (others => '0');

-- Generate arrays (inputs to cores)
gen_start_array : process (current_mult_start, start_i)
begin
    for i in 0 to NUM_MULTS*2-1 loop
        start_array(i) <= current_mult_start(i) and start_i;
    end loop;
end process gen_start_array;

gen_input_muxes : for i in 0 to NUM_MULTS*2-1 generate
    i_input_a_mux : mux_generic
        generic map(752)
        port map (a   => opa_array(i), 
	          b   => opa_i, 
		  sel => start_array(i), 
		  c   => a_next(i));
    
    i_input_b_mux  : mux_generic
        generic map(752)
        port map (a   => opb_array(i), 
	          b   => opb_i, 
		  sel => start_array(i), 
		  c   => b_next(i));
end generate gen_input_muxes;

fill_input_array : process(clk, rst)
begin
    if rst = '1' then
        for i in 0 to NUM_MULTS*2-1 loop
            opa_array(i) <= (others => '0');
            opb_array(i) <= (others => '0');
        end loop;
    elsif rising_edge(clk) then
        for i in 0 to NUM_MULTS*2-1 loop
            opa_array(i) <= a_next(i);
            opb_array(i) <= b_next(i);
        end loop;
    end if;
end process fill_input_array;

circular_fifo : process (clk,rst)
begin
    if rst = '1' then
        current_mult_start <= (0 => '1', others => '0');
        current_mult_rd <= 0;
        even_odd <= '0';
    elsif rising_edge(clk) then       
	--Reset current mult and even_odd
        if reset_even_odd_i = '1' then
            current_mult_start <= (0 => '1', others => '0');
            current_mult_rd <= 0;
            even_odd <= '1';         
        else
            -- Switch current multiplier to start by shifting bitvector
            if start_i = '1' then
                current_mult_start <= current_mult_start(NUM_MULTS*2-2 downto 0) & current_mult_start(NUM_MULTS*2-1);
            end if;     
            
            -- Switch current result read to next one upon getting a read of the multiplier result
            if res_read_i = '1' then
                if current_mult_rd < NUM_MULTS*2-1 then
                    current_mult_rd <= current_mult_rd + 1;
                else
                    current_mult_rd <= 0;
                end if;
            end if;
            --Flip even and odd bit every cycle
            even_odd <= not even_odd;
        end if;
    end if;
end process circular_fifo;           
    
-- Generate replicated multipliers
gen_mults : for i in 0 to NUM_MULTS-1 generate                
    i_MULT : mm_752
        port map (clk        => clk,
	          rst        => rst,
		  start1_i   => start_array(2*i),
		  start2_i   => start_array(2*i+1),
		  opa1_i     => a_next(2*i), 
		  opa2_i     => a_next(2*i+1),
		  opb1_i     => b_next(2*i),
		  opb2_i     => b_next(2*i+1),
		  even_odd_i => even_odd,
		  p1_o       => res_array(2*i),
		  p2_o       => res_array(2*i+1));            
end generate gen_mults;

res_o <= res_array(current_mult_rd)(751 downto 0);

end structural;
