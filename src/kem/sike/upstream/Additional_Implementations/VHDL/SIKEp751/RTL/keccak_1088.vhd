--********************************************************************************************
--* VHDL-SIKE: a speed optimized hardware implementation of the 
--*            Supersingular Isogeny Key Encapsulation scheme
--*
--*    Keccak block adapted from the Keccak Team
--*
--********************************************************************************************* 

-- The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
-- Michaël Peeters and Gilles Van Assche. For more information, feedback or
-- questions, please refer to our website: http://keccak.noekeon.org/

-- Implementation by the designers,
-- hereby denoted as "the implementer".

-- To the extent possible under law, the implementer has waived all copyright
-- and related or neighboring rights to the source code in this file.
-- http://creativecommons.org/publicdomain/zero/1.0/

library work;
	use work.keccak_globals.all;
	
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity keccak_1088 is
  
port (
    clk           : in  std_logic;
    rst           : in  std_logic;
    clear_state   : in  std_logic;
    din           : in  std_logic_vector(7 downto 0);
    din_valid     : in  std_logic;
    buffer_full   : out std_logic;
    ready         : out std_logic;
    dout          : out std_logic_vector(511 downto 0));

end keccak_1088;

architecture rtl of keccak_1088 is

--components

component keccak_round
port (
    round_in                 : in  k_state;
    round_constant_signal    : in  std_logic_vector(63 downto 0);
    round_out                : out k_state);
end component;

component keccak_round_constants_gen
port (
    round_number              : in  unsigned(4 downto 0);
    round_constant_signal_out : out std_logic_vector(63 downto 0));
end component;


component keccak_buffer_1088 
  
port (
    clk                 : in  std_logic;
    rst                 : in  std_logic;    
    din_buffer_in       : in  std_logic_vector(7 downto 0);
    din_buffer_in_valid : in  std_logic;
    ready               : in  std_logic;
    din_buffer_full     : out std_logic;
    din_buffer_out      : out std_logic_vector(1087 downto 0));
end component;

  ----------------------------------------------------------------------------
  -- Internal signal declarations
  ----------------------------------------------------------------------------

 
  signal reg_data,round_in,round_out : k_state;
  signal counter_nr_rounds           : unsigned(4 downto 0);
  signal din_buffer_full             : std_logic;
  signal round_constant_signal       : std_logic_vector(63 downto 0);
  signal din_buffer_out              : std_logic_vector(1087 downto 0);
  signal permutation_computed        : std_logic;
  signal full_state: std_logic_vector(1599 downto 0);
 
  
begin  -- Rtl

-- port map

round_map : keccak_round port map(round_in,round_constant_signal,round_out);
round_constants_gen: keccak_round_constants_gen port map(counter_nr_rounds,round_constant_signal);

buffer_in: keccak_buffer_1088 
    port map(clk                 => clk, 
             rst                 => rst,
             din_buffer_in       => din,
             din_buffer_in_valid => din_valid,
             ready               => permutation_computed,
             din_buffer_full     => din_buffer_full,
             din_buffer_out      => din_buffer_out);

i_full_state0: for row in 0 to 4 generate
	i_full_state1: for col in 0 to 4 generate
		i_full_state2: for i in 0 to 63 generate
		    full_state(64*5*row+64*col+i)<=round_in(row)(col)(i);
        end generate;
	end generate;
end generate;
			 
 -- state register and counter of the number of rounds
 
  p_main : process (clk, rst)
    
  begin  -- process p_main
    if rst = '1' then                 -- asynchronous rst (active high)
      --reg_data <= zero_state;
      		for row in 0 to 4 loop
			for col in 0 to 4 loop
				for i in 0 to 63 loop
					reg_data(row)(col)(i)<='0';
				end loop;
			end loop;
		end loop;
      counter_nr_rounds    <= (others => '0');
      permutation_computed <= '1';
    elsif clk'event and clk = '1' then  -- rising clk edge

	if (clear_state='1') then
		--reg_data <= zero_state;
		for row in 0 to 4 loop
			for col in 0 to 4 loop
				for i in 0 to 63 loop
					reg_data(row)(col)(i)<='0';
				end loop;
			end loop;
		end loop;
		counter_nr_rounds <= (others => '0');	
		permutation_computed<='1';		
	else
		if(din_buffer_full ='1' and permutation_computed='1') then
			counter_nr_rounds(4 downto 0)<= (others => '0');
			counter_nr_rounds(0)<='1';
			permutation_computed<='0';
			reg_data<= round_out;
		else
			if( counter_nr_rounds < 24 and permutation_computed='0') then			
				counter_nr_rounds <= counter_nr_rounds + 1;
				reg_data<= round_out;
							
			end if;
			if( counter_nr_rounds = 23) then
				permutation_computed<='1';
				counter_nr_rounds<= (others => '0');
			end if;
		end if;
		
	end if;
    end if;
  end process p_main;

--input mapping

--capacity part
	i01: for col in 2 to 4 generate
		i02: for i in 0 to 63 generate
			round_in(3)(col)(i)<= reg_data(3)(col)(i);
		end generate;	
	end generate;

	i03: for col in 0 to 4 generate
		i04: for i in 0 to 63 generate
			round_in(4)(col)(i)<= reg_data(4)(col)(i);
		end generate;	
	end generate;
--rate part
i10: for row in 0 to 2 generate
	i11: for col in 0 to 4 generate
		i12: for i in 0 to 63 generate
			round_in(row)(col)(i)<= reg_data(row)(col)(i) xor (din_buffer_out((row*64*5)+(col*64)+i) and (din_buffer_full and permutation_computed));
		end generate;	
	end generate;
end generate;

    i20: for col in 0 to 1 generate
        i21: for i in 0 to 63 generate
    		round_in(3)(col)(i)<= reg_data(3)(col)(i) xor (din_buffer_out((3*64*5)+(col*64)+i) and (din_buffer_full and permutation_computed));
        end generate;
    end generate;	

ready<=permutation_computed;
buffer_full<=din_buffer_full;
dout <= full_state(511 downto 0);
end rtl;
