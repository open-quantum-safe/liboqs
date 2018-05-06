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

entity sike_p751 is
    Port ( clk                     : in  std_logic;
           rst                     : in  std_logic;
           sike_cmd_i              : in  std_logic_vector(2 downto 0);
           iso_cmd_i               : in  std_logic_vector(2 downto 0);
           reg_sel_i               : in  std_logic_vector(7 downto 0);
           wr_input_sel_i          : in  std_logic;
           wr_op_sel_i             : in  std_logic_vector(1 downto 0);
           wr_word_sel_i           : in  std_logic_vector(3 downto 0);
           wr_en_i                 : in  std_logic;
           rd_reg_i                : in  std_logic;
           rd_op_sel_i             : in  std_logic_vector(1 downto 0);
           rd_word_sel_i           : in  std_logic_vector(3 downto 0);
           buffer_xor_i            : in  std_logic;
           keccak_clear_i          : in  std_logic;
           keccak_din_i            : in  std_logic_vector(7 downto 0);
           keccak_din_valid_byte_i : in  std_logic;
           keccak_word_cnt_i       : in  std_logic_vector(2 downto 0);
           keccak_word_valid_i     : in  std_logic;
           data_i                  : in  std_logic_vector (63 downto 0);
           data_o                  : out std_logic_vector (63 downto 0);
           busy_o                  : out std_logic
    );
end sike_p751; 

architecture structural of sike_p751 is

    constant INSTRUCTION_LENGTH    : integer := 32;
    constant PROG_ADDR_W           : integer := 11;
    constant PROG_LENGTH           : integer := 2**PROG_ADDR_W;
    -- SIKE Program ROM    
    COMPONENT sike_rom IS
      PORT (
        clka  : IN  std_logic;
        ena   : IN  std_logic;
        addra : IN  std_logic_vector(PROG_ADDR_W-1 DOWNTO 0);
        douta : OUT std_logic_vector(INSTRUCTION_LENGTH-1 DOWNTO 0)
      );
    END COMPONENT sike_rom;     
    
    component sike_arith_unit is
    port ( clk                     : in  std_logic;
           rst                     : in  std_logic;
           iso_cmd_i               : in  std_logic_vector(2 downto 0);
           reg_sel_i               : in  std_logic_vector(7 downto 0);
           wr_input_sel_i          : in  std_logic;
           wr_op_sel_i             : in  std_logic_vector(1 downto 0);
           wr_word_sel_i           : in  std_logic_vector(3 downto 0);
           wr_en_i                 : in  std_logic;
           rd_reg_i                : in  std_logic;
           rd_op_sel_i             : in  std_logic_vector(1 downto 0);
           rd_word_sel_i           : in  std_logic_vector(3 downto 0);
           buffer_xor_i            : in  std_logic;
           keccak_clear_i          : in  std_logic;
           keccak_din_i            : in  std_logic_vector(7 downto 0);
           keccak_din_valid_byte_i : in  std_logic;
           keccak_word_cnt_i       : in  std_logic_vector(2 downto 0);
           keccak_word_valid_i     : in  std_logic;
           data_i                  : in  std_logic_vector (63 downto 0);
           data_o                  : out std_logic_vector (63 downto 0);
           busy_o                  : out std_logic
    );
    end component sike_arith_unit; 

    --Subroutine listing
    constant NOP            : integer := 0;
    constant KEYGEN_S       : integer := 1;
    constant KEYGEN_E       : integer := 1;
    constant ENCAPSULATE_S  : integer := 2;
    constant ENCAPSULATE_E  : integer := 865;
    constant DECAPSULATE_S  : integer := 866;
    constant DECAPSULATE_E  : integer := 1444;
    constant DECAPBAD_S     : integer := 1445;
    constant DECAPBAD_E     : integer := 1729;
    constant DECAPGOOD_S    : integer := 1730;
    constant DECAPGOOD_E    : integer := 2014;

    signal prog_cntr : integer range 0 to PROG_LENGTH := NOP;   
    signal stall_cntr : integer range 0 to 150;

    signal prog_addr : std_logic_vector(PROG_ADDR_W-1 downto 0);
    signal prog_line : std_logic_vector(INSTRUCTION_LENGTH-1 downto 0);

    signal iso_busy_s : std_logic;
    signal busy_s,busy_r,busy_d1,busy_d2 : std_logic;
    
    signal iso_cmd_s : std_logic_vector(2 downto 0);
    signal reg_sel_s : std_logic_vector(7 downto 0);
    signal wr_input_sel_s : std_logic;
    signal wr_op_sel_s : std_logic_vector(1 downto 0);
    signal wr_word_sel_s : std_logic_vector(3 downto 0);
    signal wr_en_s : std_logic;
    signal rd_reg_s : std_logic;
    signal rd_op_sel_s : std_logic_vector(1 downto 0);
    signal rd_word_sel_s : std_logic_vector(3 downto 0);
    signal buffer_xor_s : std_logic;
    signal keccak_clear_s : std_logic;
    signal keccak_din_s : std_logic_vector(7 downto 0);
    signal keccak_din_valid_byte_s : std_logic;
    signal keccak_word_cnt_s : std_logic_vector(2 downto 0);
    signal keccak_word_valid_s : std_logic;
    signal cycle_count_r : std_logic_vector(1 downto 0);
    signal cycle_count_d1 : std_logic_vector(1 downto 0);
    signal w_one_2 : std_logic_vector(1 downto 0);

begin
    w_one_2 <= "01";
    -- Program ROM
    prog_addr <= std_logic_vector(to_unsigned(prog_cntr,PROG_ADDR_W));

    sike_input_sel : process(prog_line, busy_s, cycle_count_r, cycle_count_d1,
                             iso_cmd_i, reg_sel_i, wr_input_sel_i,
                             wr_op_sel_i, wr_word_sel_i, wr_en_i, rd_reg_i, rd_op_sel_i,
                             rd_word_sel_i, buffer_xor_i, keccak_clear_i, keccak_din_i,
                             keccak_din_valid_byte_i, keccak_word_cnt_i, keccak_word_valid_i)
    begin
        if (busy_s = '1') then
            if ((cycle_count_r = "11") and (cycle_count_d1 = "10")) then --Only pulses for command signals
                iso_cmd_s               <= prog_line(31 downto 29);
                --iso_cmd_s               <= (others => '0');
                keccak_word_valid_s     <= prog_line(28);
                wr_en_s                 <= prog_line(7); 
                keccak_din_valid_byte_s <= prog_line(24);  
                keccak_clear_s          <= prog_line(23);
                buffer_xor_s            <= prog_line(22); 
            else
                iso_cmd_s               <= (others => '0');
                keccak_word_valid_s     <= '0';
                wr_en_s                 <= '0';
                keccak_din_valid_byte_s <= '0';
                keccak_clear_s          <= '0';
                buffer_xor_s            <= '0'; 
            end if;
            reg_sel_s               <= prog_line(21 downto 14); 
            wr_input_sel_s          <= '1'; 
            wr_op_sel_s             <= prog_line(13 downto 12); 
            wr_word_sel_s           <= prog_line(11 downto 8);
            rd_reg_s                <= prog_line(6);  
            rd_op_sel_s             <= prog_line(5 downto 4); 
            rd_word_sel_s           <= prog_line(3 downto 0); 
            keccak_din_s            <= prog_line(21 downto 14);
            keccak_word_cnt_s       <= prog_line(27 downto 25);
             
        else
            iso_cmd_s               <=  iso_cmd_i;  
            reg_sel_s               <=  reg_sel_i;               
            wr_input_sel_s          <=  wr_input_sel_i;          
            wr_op_sel_s             <=  wr_op_sel_i;             
            wr_word_sel_s           <=  wr_word_sel_i;           
            wr_en_s                 <=  wr_en_i;                 
            rd_reg_s                <=  rd_reg_i;                
            rd_op_sel_s             <=  rd_op_sel_i;             
            rd_word_sel_s           <=  rd_word_sel_i;           
            buffer_xor_s            <=  buffer_xor_i;            
            keccak_clear_s          <=  keccak_clear_i;          
            keccak_din_s            <=  keccak_din_i;            
            keccak_din_valid_byte_s <=  keccak_din_valid_byte_i; 
            keccak_word_cnt_s       <=  keccak_word_cnt_i;       
            keccak_word_valid_s     <=  keccak_word_valid_i;     
        end if;
    end process sike_input_sel;

    i_sike_rom : sike_rom
        port map (clka  => clk,
	          ena   => '1',
	          addra => prog_addr,
	          douta => prog_line);
	          
    -- SIKE Control logic
    i_sike : sike_arith_unit
        port map (clk                     => clk,
	          rst                     => rst,
	          iso_cmd_i               => iso_cmd_s,              
                  reg_sel_i               => reg_sel_s,              
                  wr_input_sel_i          => wr_input_sel_s,         
                  wr_op_sel_i             => wr_op_sel_s,            
                  wr_word_sel_i           => wr_word_sel_s,          
                  wr_en_i                 => wr_en_s,                
                  rd_reg_i                => rd_reg_s,               
                  rd_op_sel_i             => rd_op_sel_s,            
                  rd_word_sel_i           => rd_word_sel_s,          
                  buffer_xor_i            => buffer_xor_s,           
                  keccak_clear_i          => keccak_clear_s,         
                  keccak_din_i            => keccak_din_s,           
                  keccak_din_valid_byte_i => keccak_din_valid_byte_s,
                  keccak_word_cnt_i       => keccak_word_cnt_s,      
		  keccak_word_valid_i     => keccak_word_valid_s,    
		  data_i                  => data_i,
		  data_o                  => data_o,
                  busy_o                  => iso_busy_s);
		
    -- Program subroutine flow for sike operations
    sike_flow : process (clk,rst)
    begin
        if rst = '1' then
            prog_cntr <= 0;
            cycle_count_r <= "00";
            cycle_count_d1 <= "00";
        elsif rising_edge(clk) then
            cycle_count_d1 <= cycle_count_r;
            if iso_busy_s = '1' then --Stalled until command is done
                prog_cntr <= prog_cntr;
                cycle_count_r <= cycle_count_r;
            elsif ((busy_s = '1') and (cycle_count_r /= "11")) then --When not NOP
                    cycle_count_r <= std_logic_vector(unsigned(cycle_count_r) + unsigned(w_one_2));
            else
                cycle_count_r <= "00";
                case prog_cntr is
		    -- Waiting for new valid command    
                    when NOP =>
                        case sike_cmd_i is
                            when "001"  => prog_cntr <= KEYGEN_S;
                            when "010"  => prog_cntr <= ENCAPSULATE_S;
                            when "011"  => prog_cntr <= DECAPSULATE_S;
                            when "100"  => prog_cntr <= DECAPBAD_S;    -- cipher texts do not match
                            when "101"  => prog_cntr <= DECAPGOOD_S;   -- (cipher texts match)
                            when others => prog_cntr <= NOP;
                        end case;
                    when KEYGEN_E      => prog_cntr <= NOP;
                    when ENCAPSULATE_E => prog_cntr <= NOP;
                    when DECAPSULATE_E => prog_cntr <= NOP;
                    when DECAPBAD_E    => prog_cntr <= NOP;
                    when DECAPGOOD_E   => prog_cntr <= NOP;
                    when others        => prog_cntr <= prog_cntr + 1;
                end case;
            end if;
        end if;
    end process sike_flow;

    busy_s <= '0' when prog_cntr = NOP else '1';
    busy_o <= busy_s or busy_r or busy_d1 or busy_d2 or iso_busy_s;
    
    program_pipeline : process (clk,rst)
    begin
        if rst = '1' then
            busy_r  <= '0';
            busy_d1 <= '0';
            busy_d2 <= '0';
        elsif rising_edge(clk) then
            busy_r  <= busy_s;
            busy_d1 <= busy_r;
            busy_d2 <= busy_d1;
        end if;
    end process program_pipeline;

end structural;
