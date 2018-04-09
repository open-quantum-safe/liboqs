--********************************************************************************************
--* VHDL-SIKE: a speed optimized hardware implementation of the 
--*            Supersingular Isogeny Key Encapsulation scheme
--*
--*    Keccak buffer adapted from the Keccak Team
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



entity keccak_buffer_1088 is
  
  port (
    clk                 : in  std_logic;
    rst                 : in  std_logic;    
    din_buffer_in       : in  std_logic_vector(7 downto 0);
    din_buffer_in_valid : in  std_logic;
    ready               : in  std_logic;
    din_buffer_full     : out std_logic;
    din_buffer_out      : out std_logic_vector(1087 downto 0));

end keccak_buffer_1088;

architecture rtl of keccak_buffer_1088 is

  signal buffer_full: std_logic; --mode=0 input mode/ mode=1 output mode
  signal count_in_words : unsigned(7 downto 0); --0 to 135
  signal buffer_data: std_logic_vector(1087 downto 0);
  
begin  -- Rtl
 -- buffer
 p_main : process (clk, rst)
    
  begin  -- process p_main
    if rst = '1' then                 -- asynchronous rst (active high)
      buffer_full    <='0';
      count_in_words <= (others => '0');
      buffer_data    <= (others => '0');
      
    elsif clk'event and clk = '1' then  -- rising clk edge
	
	   --input mode
	if(buffer_full='1' and ready ='1')  then
	    buffer_full<='0';
	    count_in_words<= (others=>'0');	
	else	
	    if (din_buffer_in_valid='1' and buffer_full='0') then
		--shift buffer
    		for i in 0 to 134 loop
		    buffer_data( 7+(i*8) downto 0+(i*8) )<=buffer_data( 15+(i*8) downto 8+(i*8) );			
		end loop;
				
		--insert new input
		buffer_data(1087 downto 1080) <= din_buffer_in;
		if (count_in_words=135) then
		    -- buffer full ready for being absorbed by the permutation
    	            buffer_full <= '1';
	    	    count_in_words<= (others=>'0');
		else
		    -- increment count_in_words
		    count_in_words <= count_in_words + 1;				
		end if;		
		--  end if;
	    end if;
	end if;
    end if;
  end process p_main;

din_buffer_out<=buffer_data;
din_buffer_full<=buffer_full;

end rtl;
