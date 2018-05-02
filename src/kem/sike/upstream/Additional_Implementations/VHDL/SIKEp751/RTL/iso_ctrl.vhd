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

entity iso_ctrl is
    Port ( clk         : in  std_logic;
           rst         : in  std_logic;
           iso_cmd_i   : in  std_logic_vector(2 downto 0);
           digit_in    : in  std_logic;
           instr_o     : out std_logic_vector(24 downto 0);
           req_digit_o : out std_logic;
           alice_o     : out std_logic;
           busy_o      : out std_logic);
end iso_ctrl;
architecture structural of iso_ctrl is

    constant INSTRUCTION_LENGTH    : integer := 26;
    constant PROG_ADDR_W           : integer := 16;
    constant PROG_LENGTH           : integer := 2**PROG_ADDR_W;

    -- Strategy lookup ROM
    COMPONENT lut43_table
      PORT (
        clka  : IN  std_logic;
        ena   : IN  std_logic;
        addra : IN  std_logic_vector(10 DOWNTO 0);
        douta : OUT std_logic_vector(8 DOWNTO 0)
      );
    END COMPONENT;

    -- Program ROM    
    COMPONENT prog_rom IS
      PORT (
        clka  : IN  std_logic;
        ena   : IN  std_logic;
        addra : IN  std_logic_vector(PROG_ADDR_W-1 DOWNTO 0);
        douta : OUT std_logic_vector(INSTRUCTION_LENGTH-1 DOWNTO 0)
      );
    END COMPONENT prog_rom;     

    --Subroutine listing
    constant NOP       : integer := 0; --Also resets even_odd
    constant ISO_LOOP_CHK_S         : integer := 1;
    constant ISO_LOOP_CHK_E         : integer := 1;
    constant A1_INIT_S              : integer := 2;
    constant A1_INIT_E              : integer := 77;
    constant B1_INIT_S              : integer := 78;
    constant B1_INIT_E              : integer := 153;
    constant THREE_PT_LADDER_INIT_S : integer := 154;
    constant THREE_PT_LADDER_INIT_E : integer := 187;
    constant THREE_PT_LADDER1_S     : integer := 188;
    constant THREE_PT_LADDER1_E     : integer := 553;
    constant THREE_PT_LADDER2_S     : integer := 554;
    constant THREE_PT_LADDER2_E     : integer := 919;
    constant ISO_INIT_ALICE_R1_S    : integer := 920;
    constant ISO_INIT_ALICE_R1_E    : integer := 930;
    constant ISO_INIT_BOB_R1_S      : integer := 931;
    constant ISO_INIT_BOB_R1_E      : integer := 941;
    constant ISO_INIT_ALICE_R2_S    : integer := 942;
    constant ISO_INIT_ALICE_R2_E    : integer := 952;
    constant ISO_INIT_BOB_R2_S      : integer := 953;
    constant ISO_INIT_BOB_R2_E      : integer := 968;
    constant ISO_REDUCE_S           : integer := 969;
    constant ISO_REDUCE_E           : integer := 982;
    constant ISO_SPLIT_S            : integer := 983;
    constant ISO_SPLIT_E            : integer := 996;
    constant MONT_QUAD_S            : integer := 997;
    constant MONT_QUAD_E            : integer := 1423;
    constant MONT_TRIPLE_S          : integer := 1424;
    constant MONT_TRIPLE_E          : integer := 1819;
    constant GET_4_ISO_S            : integer := 1820;
    constant GET_4_ISO_E            : integer := 1918;
    constant GET_3_ISO_S            : integer := 1919;
    constant GET_3_ISO_E            : integer := 2112;
    constant ALICE_R1_POST_ISO_S    : integer := 2113;
    constant ALICE_R1_POST_ISO_E    : integer := 2131;
    constant BOB_R1_POST_ISO_S      : integer := 2132;
    constant BOB_R1_POST_ISO_E      : integer := 2150;
    constant INV_3_WAY_PART1_S      : integer := 2151;
    constant INV_3_WAY_PART1_E      : integer := 2289;
    constant INV_3_WAY_PART2_S      : integer := 2290;
    constant INV_3_WAY_PART2_E      : integer := 2384;
    constant ALICE_R1_END_S         : integer := 2385;
    constant ALICE_R1_END_E         : integer := 2508;
    constant BOB_R1_END_S           : integer := 2509;
    constant BOB_R1_END_E           : integer := 2632;
    constant A2_INIT_S              : integer := 2633;
    constant A2_INIT_E              : integer := 2650;
    constant B2_INIT_S              : integer := 2651;
    constant B2_INIT_E              : integer := 2668;
    constant GET_A_PART1_S          : integer := 2669;
    constant GET_A_PART1_E          : integer := 2840;
    constant GET_A_PART2_S          : integer := 2841;
    constant GET_A_PART2_E          : integer := 2890;
    constant ALICE_R2_POST_ISO_S    : integer := 2891;
    constant ALICE_R2_POST_ISO_E    : integer := 2920;
    constant BOB_R2_POST_ISO_S      : integer := 2921;
    constant BOB_R2_POST_ISO_E      : integer := 2950;
    constant J_INV_PART1_S          : integer := 2951;
    constant J_INV_PART1_E          : integer := 3191;
    constant ALICE_J_INV_END_S      : integer := 3192;
    constant ALICE_J_INV_END_E      : integer := 3241;
    constant BOB_J_INV_END_S        : integer := 3242;
    constant BOB_J_INV_END_E        : integer := 3291;
    constant INV2_S                 : integer := 3292;
    constant INV2_E                 : integer := 9920;
    constant EVAL3_X1_S             : integer := 9921;
    constant EVAL3_X1_E             : integer := 10119;
    constant EVAL3_X2_S             : integer := 10120;
    constant EVAL3_X2_E             : integer := 10483;
    constant EVAL3_OPP_S            : integer := 10484;
    constant EVAL3_OPP_E            : integer := 11098;
    constant EVAL3_X3_S             : integer := 11099;
    constant EVAL3_X3_E             : integer := 11713;
    constant EVAL3_X4_S             : integer := 11714;
    constant EVAL3_X4_E             : integer := 12540;
    constant EVAL3_X5_S             : integer := 12541;
    constant EVAL3_X5_E             : integer := 13522;
    constant EVAL3_X6_S             : integer := 13523;
    constant EVAL3_X6_E             : integer := 14702;
    constant EVAL3_X7_S             : integer := 14703;
    constant EVAL3_X7_E             : integer := 16109;
    constant EVAL3_X8_S             : integer := 16110;
    constant EVAL3_X8_E             : integer := 17716;
    constant EVAL3_X9_S             : integer := 17717;
    constant EVAL3_X9_E             : integer := 19476;
    constant EVAL3_X10_S            : integer := 19477;
    constant EVAL3_X10_E            : integer := 21433;
    constant EVAL3_X11_S            : integer := 21434;
    constant EVAL3_X11_E            : integer := 23552;
    constant EVAL3_X12_S            : integer := 23553;
    constant EVAL3_X12_E            : integer := 25851;
    constant EVAL4_X1_S             : integer := 25852;
    constant EVAL4_X1_E             : integer := 26150;
    constant EVAL4_X2_S             : integer := 26151;
    constant EVAL4_X2_E             : integer := 26675;
    constant EVAL4_X3_S             : integer := 26676;
    constant EVAL4_X3_E             : integer := 27517;
    constant EVAL4_OPP_S            : integer := 27518;
    constant EVAL4_OPP_E            : integer := 28359;
    constant EVAL4_X4_S             : integer := 28360;
    constant EVAL4_X4_E             : integer := 29445;
    constant EVAL4_X5_S             : integer := 29446;
    constant EVAL4_X5_E             : integer := 30780;
    constant EVAL4_X6_S             : integer := 30781;
    constant EVAL4_X6_E             : integer := 32368;
    constant EVAL4_X7_S             : integer := 32369;
    constant EVAL4_X7_E             : integer := 34317;
    constant EVAL4_X8_S             : integer := 34318;
    constant EVAL4_X8_E             : integer := 36467;
    constant EVAL4_X9_S             : integer := 36468;
    constant EVAL4_X9_E             : integer := 38859;
    constant EVAL4_X10_S            : integer := 38860;
    constant EVAL4_X10_E            : integer := 41547;
    constant EVAL4_X11_S            : integer := 41548;
    constant EVAL4_X11_E            : integer := 44458;
    constant EVAL4_X12_S            : integer := 44459;
    constant EVAL4_X12_E            : integer := 44459;


    signal prog_cntr : integer range 0 to PROG_LENGTH := NOP;   
    signal stall_cntr : integer range 0 to 150;
    
    -- Program pipeline signals
    signal prog_addr : std_logic_vector(PROG_ADDR_W-1 downto 0);
    signal prog_line : std_logic_vector(INSTRUCTION_LENGTH-1 downto 0);
    signal prog_line_pipe1 : std_logic_vector(INSTRUCTION_LENGTH-1 downto 0);
    signal prog_line_pipe2 : std_logic_vector(INSTRUCTION_LENGTH-1 downto 0);
    
    signal first_round : std_logic;
    signal start_iso : std_logic;
    signal alice_round : std_logic;
    signal busy_s,busy_r,busy_d1,busy_d2 : std_logic;
    
    --Signals for splits
    signal num_mults : integer range 0 to 1023;
    
    signal mont_count : integer range 0 to 511;
    signal iso_count : integer range 0 to 511; --MAX
    constant ALICE_MONT_COUNT  : integer := 372; --372; --372; --31,250,372
    constant BOB_MONT_COUNT    : integer := 379; --32,253,379
    constant ALICE_ISO_COUNT   : integer := 186; --125,186
    constant BOB_ISO_COUNT     : integer := 239; --159,239
    
    signal iso_index : integer range 0 to 1023; --index
    signal iso_row   : integer range 0 to 1023; --row
    signal iso_m_addr: integer range 0 to 1023; --MAX-index-row+1
    signal iso_m     : integer range 0 to 1023; --m := splits[MAX-index-row+1]
    signal iso_loop  : integer range 0 to 1023; --MAX - row
    signal iso_done : std_logic;
    
    signal queue_size_r : integer range 0 to 15; --0 to 16
    signal queue_size_d1 : integer range 0 to 15;
    signal queue_size_d2 : integer range 0 to 15;
    signal queue_size_d3 : integer range 0 to 15;
    signal queue_size_d4 : integer range 0 to 15;
    signal queue_pointer : integer range 0 to 31; --What address the queue is pointing at
    signal lut43_out : std_logic_vector(8 downto 0);
    signal lut43_addr : std_logic_vector(10 downto 0);
    signal stalled : std_logic;
    signal stalled_pipe1 : std_logic;
    signal finish_get_A : std_logic;
    
    type index_array is array (0 to 15) of integer range 0 to 1023; --Array of indexes
    signal indices : index_array;

begin
    -- Strategy Lookup Table
    lut43_addr(10) <= not alice_round; --Alice's LUT is first 512 entries
    lut43_addr(9 downto 0) <= std_logic_vector(to_unsigned(iso_m_addr,10));
    i_strategy_lookup : lut43_table
        port map (clka  => clk, 
	              ena   => '1', 
		          addra => lut43_addr, 
		          douta => lut43_out);
        
    -- Program ROM
    prog_addr <= std_logic_vector(to_unsigned(prog_cntr,PROG_ADDR_W));

    i_prog_rom : prog_rom
        port map (clka  => clk,
	              ena   => '1',
		          addra => prog_addr,
		          douta => prog_line);

    alice_o <= alice_round;
    -- Program subroutine flow for isogen and isoex
    program_flow : process (clk,rst)
    begin
        if rst = '1' then
            prog_cntr <= 0;
            stall_cntr <= 0;
            req_digit_o <= '0';
            first_round <= '0';
            alice_round <= '0';
            start_iso <= '0';
            mont_count <= 0;
            iso_count <= 0;
            queue_size_r <= 0;
            queue_size_d1 <= 0;
            queue_size_d2 <= 0;
            queue_size_d3 <= 0;
            queue_size_d4 <= 0;
            queue_pointer <= 0;
            stalled <= '0';
            stalled_pipe1 <= '0';
            
            iso_m_addr <= 0;
            iso_loop <= 0;
            iso_m <= 0;
            iso_row <= 0;
            iso_done <= '0';
            
            finish_get_A <= '0';
            
        elsif rising_edge(clk) then
            queue_size_d1 <= queue_size_r;
            queue_size_d2 <= queue_size_d1;
            queue_size_d3 <= queue_size_d2;
            queue_size_d4 <= queue_size_d3;
            queue_pointer <= queue_size_d4 + 4; --Use delayed queue size
            iso_m_addr <= iso_count - iso_index - iso_row;
            iso_loop <= iso_count - iso_row;
            iso_m <= to_integer(unsigned(lut43_out));            
            stalled_pipe1 <= stalled;
            if stalled = '1' and stall_cntr > 0 then
                req_digit_o <= '0';
                stall_cntr <= stall_cntr - 1;
            elsif (prog_line(25) = '1') and stalled = '0' and stalled_pipe1 = '0' then --stall counter to prevent more reads from program ROM
                req_digit_o <= '0';
                stalled <= '1';
                stall_cntr <= to_integer(unsigned(prog_line(7 downto 0)));
            elsif (stalled = '0') or (stalled = '1' and stall_cntr = 0) then
                stalled <= '0';
                case prog_cntr is
		    -- Waiting for new valid command    
                    when NOP =>
                        
                        case iso_cmd_i is
                            when "001" => prog_cntr <= A1_INIT_S; -- Alice1
                                          first_round <= '1';
                                          alice_round <= '1';
                                          start_iso <= '1';
                                          mont_count <= ALICE_MONT_COUNT;
                                          iso_count <= ALICE_ISO_COUNT;
                                          iso_done <= '0';
                                          iso_index <= 0;
                                          iso_row <= 1;
                                          queue_size_r <= 0;                                    
                            when "010" => prog_cntr <= B1_INIT_S; -- Bob1
                                          first_round <= '1';
                                          alice_round <= '0';
                                          mont_count <= BOB_MONT_COUNT;
                                          iso_count <= BOB_ISO_COUNT;
                                          iso_done <= '0';
                                          iso_index <= 0;
                                          iso_row <= 1;
                                          queue_size_r <= 0;
                            when "011" => prog_cntr <= A2_INIT_S; -- Alice2
                                          first_round <= '0';
                                          alice_round <= '1';
                                          start_iso <= '1';
                                          mont_count <= ALICE_MONT_COUNT;
                                          iso_count <= ALICE_ISO_COUNT;
                                          iso_done <= '0';
                                          iso_index <= 0;
                                          iso_row <= 1;
                                          queue_size_r <= 0;
                                          finish_get_A <= '1';               
                            when "100" => prog_cntr <= B2_INIT_S; -- Bob2
                                          first_round <= '0';
                                          alice_round <= '0';
                                          mont_count <= BOB_MONT_COUNT;
                                          iso_count <= BOB_ISO_COUNT;
                                          iso_done <= '0';
                                          iso_index <= 0;
                                          iso_row <= 1;
                                          queue_size_r <= 0;
                                          finish_get_A <= '1';
                            when others => prog_cntr <= NOP;
                        end case;
                        req_digit_o <= '0';
                    
                    when A1_INIT_E =>
                        prog_cntr <= THREE_PT_LADDER_INIT_S;                    
                    when B1_INIT_E =>
                        prog_cntr <= THREE_PT_LADDER_INIT_S;
                    
                    when A2_INIT_E =>
                        prog_cntr <= GET_A_PART1_S;
                    when B2_INIT_E =>
                        prog_cntr <= GET_A_PART1_S;

                    when GET_A_PART1_E =>
                        prog_cntr <= INV2_S;
                    when GET_A_PART2_E =>
                        prog_cntr <= THREE_PT_LADDER_INIT_S;
                        finish_get_A <= '0';

                    -- DOUBLE POINT MULTIPLICATION
                    
                    when THREE_PT_LADDER_INIT_E =>
                        mont_count <= mont_count - 1;
                        req_digit_o <= '1';
                        case digit_in is
                            when '0' =>
                                prog_cntr <= THREE_PT_LADDER1_S;
                            when '1' =>
                                prog_cntr <= THREE_PT_LADDER2_S;
                            when others =>
                        end case;
                    
                    when THREE_PT_LADDER1_S => 
                        req_digit_o <= '0';
                        prog_cntr <= prog_cntr + 1;
                    
                    when THREE_PT_LADDER1_E =>
                        if mont_count = 0 then
                            if first_round = '1' then   
                                if alice_round = '1' then
                                    prog_cntr <= ISO_INIT_ALICE_R1_S;
                                else
                                    prog_cntr <= ISO_INIT_BOB_R1_S;
                                end if;
                            else
                                if alice_round = '1' then
                                    prog_cntr <= ISO_INIT_ALICE_R2_S;
                                else
                                    prog_cntr <= ISO_INIT_BOB_R2_S;
                                end if;                            
                            end if;
                        else
                            req_digit_o <= '1';   
                            case digit_in is
                                when '0' =>
                                    prog_cntr <= THREE_PT_LADDER1_S;
                                when '1' =>
                                    prog_cntr <= THREE_PT_LADDER2_S;
                                when others =>
                            end case;
                            mont_count <= mont_count - 1;
                        end if;  
                        
                    when THREE_PT_LADDER2_S => 
                        req_digit_o <= '0';
                        prog_cntr <= prog_cntr + 1;
                    
                    when THREE_PT_LADDER2_E =>
                        if mont_count = 0 then
                            if first_round = '1' then   
                                if alice_round = '1' then
                                    prog_cntr <= ISO_INIT_ALICE_R1_S;
                                else
                                    prog_cntr <= ISO_INIT_BOB_R1_S;
                                end if;
                            else
                                if alice_round = '1' then
                                    prog_cntr <= ISO_INIT_ALICE_R2_S;
                                else
                                    prog_cntr <= ISO_INIT_BOB_R2_S;
                                end if;                            
                            end if;
                        else
                            req_digit_o <= '1';   
                            case digit_in is
                                when '0' =>
                                    prog_cntr <= THREE_PT_LADDER1_S;
                                when '1' =>
                                    prog_cntr <= THREE_PT_LADDER2_S;
                                when others =>
                            end case;
                            mont_count <= mont_count - 1;
                        end if;  

                    when ISO_INIT_ALICE_R1_E =>
                        prog_cntr <= ISO_SPLIT_S;
                    when ISO_INIT_BOB_R1_E =>
                        prog_cntr <= ISO_SPLIT_S;
                    when ISO_INIT_ALICE_R2_E =>
                        prog_cntr <= ISO_SPLIT_S;
                    when ISO_INIT_BOB_R2_E =>
                        prog_cntr <= ISO_SPLIT_S;                        
                    -- SHIFT ISO registers and increment queue size
                    when ISO_REDUCE_E =>
                        prog_cntr <= ISO_LOOP_CHK_S;
                    
                    when ISO_LOOP_CHK_E =>
                        if iso_index >= iso_loop then
                            if alice_round = '1' then
                                prog_cntr <= GET_4_ISO_S;
                            else
                                prog_cntr <= GET_3_ISO_S;
                            end if;
                        else
                            prog_cntr <= ISO_SPLIT_S;
                        end if;                    
                        
                    when ISO_SPLIT_S =>
                        prog_cntr <= prog_cntr + 1;
                        indices(queue_size_r) <= iso_index;
                    when ISO_SPLIT_E => 
                        prog_cntr <= prog_cntr + 1;
                        queue_size_r <= queue_size_r + 1;
                        num_mults <= iso_m - 1;
                        iso_index <= iso_index + iso_m;
                        if alice_round = '1' then
                            prog_cntr <= MONT_QUAD_S;
                        else
                            prog_cntr <= MONT_TRIPLE_S;
                        end if;

                    when MONT_QUAD_E =>
                        if num_mults = 0 then
                            prog_cntr <= ISO_LOOP_CHK_S;
                        else
							prog_cntr <= MONT_QUAD_S;
                            num_mults <= num_mults - 1;
                        end if;
                    when MONT_TRIPLE_E =>
                        if num_mults = 0 then
                            prog_cntr <= ISO_LOOP_CHK_S;
                        else
                            prog_cntr <= MONT_TRIPLE_S;
                            num_mults <= num_mults - 1;
                        end if;
                    
                    when GET_4_ISO_E =>
                        if queue_size_r /= 0 then
                            iso_index <= indices(queue_size_r-1);
                            indices(queue_size_r-1) <= 0;
                            queue_size_r <= queue_size_r - 1;
                        end if;
                        iso_row <= iso_row + 1;
                        case queue_size_r is
                            when 0 =>
                                case first_round is
                                    when '1' =>
                                        prog_cntr <= EVAL4_OPP_S;
                                        iso_done <= '1';
                                    when '0' =>
                                        prog_cntr <= ALICE_R2_POST_ISO_S;
                                    when others =>
                                end case;
                            when 1 =>
                                prog_cntr <= EVAL4_X1_S;
                            when 2 =>
                                prog_cntr <= EVAL4_X2_S;
                            when 3 =>
                                prog_cntr <= EVAL4_X3_S;
                            when 4 =>
                                prog_cntr <= EVAL4_X4_S;
                            when 5 =>
                                prog_cntr <= EVAL4_X5_S;
                            when 6 =>
                                prog_cntr <= EVAL4_X6_S;
                            when 7 =>
                                prog_cntr <= EVAL4_X7_S;
                            when 8 =>
                                prog_cntr <= EVAL4_X8_S;
                            when 9 =>
                                prog_cntr <= EVAL4_X9_S;
                            when 10 =>
                                prog_cntr <= EVAL4_X10_S;
                            when 11 =>
                                prog_cntr <= EVAL4_X11_S;
                            when 12 =>
                                prog_cntr <= EVAL4_X12_S;                            
                            when others =>
                       end case;                    
                    when GET_3_ISO_E =>
                        if queue_size_r /= 0 then
                            iso_index <= indices(queue_size_r-1);
                            indices(queue_size_r-1) <= 0;
                            queue_size_r <= queue_size_r - 1;
                        end if;
                        iso_row <= iso_row + 1;
                        case queue_size_r is
                            when 0 =>
                                case first_round is
                                    when '1' =>
                                        prog_cntr <= EVAL3_OPP_S;
                                        iso_done <= '1';
                                    when '0' =>
                                        prog_cntr <= BOB_R2_POST_ISO_S;
                                    when others =>
                                end case;
                            when 1 =>
                                prog_cntr <= EVAL3_X1_S;
                            when 2 =>
                                prog_cntr <= EVAL3_X2_S;
                            when 3 =>
                                prog_cntr <= EVAL3_X3_S;
                            when 4 =>
                                prog_cntr <= EVAL3_X4_S;
                            when 5 =>
                                prog_cntr <= EVAL3_X5_S;
                            when 6 =>
                                prog_cntr <= EVAL3_X6_S;
                            when 7 =>
                                prog_cntr <= EVAL3_X7_S;
                            when 8 =>
                                prog_cntr <= EVAL3_X8_S;
                            when 9 =>
                                prog_cntr <= EVAL3_X9_S;
                            when 10=>
                                prog_cntr <= EVAL3_X10_S;
                            when 11 =>
                                prog_cntr <= EVAL3_X11_S;
                            when 12=>
                                prog_cntr <= EVAL3_X12_S;
                            when others =>   
                       end case;
                   when EVAL4_X1_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;     
                   when EVAL4_X2_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;
                   when EVAL4_X3_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;      
                   when EVAL4_X4_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;
                   when EVAL4_X5_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;
                   when EVAL4_X6_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;
                   when EVAL4_X7_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;     
                   when EVAL4_X8_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;    
                   when EVAL4_X9_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case; 
                   when EVAL4_X10_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;     
                   when EVAL4_X11_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;    
                   when EVAL4_X12_E =>
                       case first_round is
                           when '0' =>
                               prog_cntr <= ISO_REDUCE_S;
                           when '1' =>
                               prog_cntr <= EVAL4_OPP_S;
                           when others =>
                       end case;                          
                                               
                    when EVAL3_X1_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;     
                    when EVAL3_X2_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;
                    when EVAL3_X3_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;      
                    when EVAL3_X4_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;
                    when EVAL3_X5_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;
                    when EVAL3_X6_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;
                    when EVAL3_X7_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;     
                    when EVAL3_X8_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;    
                    when EVAL3_X9_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;        
                    when EVAL3_X10_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;
                    when EVAL3_X11_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;      
                    when EVAL3_X12_E =>
                        case first_round is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= EVAL3_OPP_S;
                            when others =>
                        end case;                                                     

                    when EVAL4_OPP_E =>
                        case iso_done is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= ALICE_R1_POST_ISO_S;
                            when others =>
                        end case;
                   
                    when EVAL3_OPP_E =>
                        case iso_done is
                            when '0' =>
                                prog_cntr <= ISO_REDUCE_S;
                            when '1' =>
                                prog_cntr <= BOB_R1_POST_ISO_S;
                            when others =>
                        end case;
                        
                    when ALICE_R1_POST_ISO_E =>
                        prog_cntr <= INV_3_WAY_PART1_S;
                    when BOB_R1_POST_ISO_E =>
                        prog_cntr <= INV_3_WAY_PART1_S;
                        
                    when ALICE_R2_POST_ISO_E =>
                        prog_cntr <= J_INV_PART1_S;
                    when BOB_R2_POST_ISO_E =>
                        prog_cntr <= J_INV_PART1_S;                        
                        
                    when INV_3_WAY_PART1_E =>
                        prog_cntr <= INV2_S;
                    when J_INV_PART1_E =>
                        prog_cntr <= INV2_S;
                    when INV2_E =>
                        case first_round is
                            when '0' =>
                                if finish_get_A = '1' then
                                    prog_cntr <= GET_A_PART2_S;
                                else
                                    case alice_round is
                                        when '0' =>
                                            prog_cntr <= BOB_J_INV_END_S;
                                        when '1' =>
                                            prog_cntr <= ALICE_J_INV_END_S;
                                        when others =>
                                    end case;                               
                                end if;
                            when '1' =>
                                prog_cntr <= INV_3_WAY_PART2_S;
                            when others =>
                        end case;
                    when INV_3_WAY_PART2_E =>
                        case alice_round is
                            when '0' =>
                                prog_cntr <= BOB_R1_END_S;
                            when '1' =>
                                prog_cntr <= ALICE_R1_END_S;
                            when others =>
                        end case;
                    when ALICE_R1_END_E =>
                        prog_cntr <= NOP;
                    when BOB_R1_END_E =>
                        prog_cntr <= NOP;
                    when ALICE_J_INV_END_E =>
                        prog_cntr <= NOP;
                    when BOB_J_INV_END_E =>
                        prog_cntr <= NOP;
                    when others => prog_cntr <= prog_cntr + 1;
                end case;
            end if;
        end if;
    end process program_flow;
    
    busy_s <= '0' when prog_cntr = NOP else '1';
    busy_o <= busy_s or busy_r or busy_d1 or busy_d2;
    
    program_pipeline : process (clk,rst)
    begin
        if rst = '1' then
            prog_line_pipe1 <= (others => '0');
            prog_line_pipe2 <= (others => '0');
            busy_r  <= '0';
            busy_d1 <= '0';
            busy_d2 <= '0';
        elsif rising_edge(clk) then
            if stalled_pipe1 = '1' then
                prog_line_pipe1 <= prog_line_pipe1;
                prog_line_pipe2 <= prog_line_pipe2;
            else
                prog_line_pipe1 <= prog_line;
                prog_line_pipe2 <= prog_line_pipe1;
            end if;
            busy_r  <= busy_s;
            busy_d1 <= busy_r;
            busy_d2 <= busy_d1;
        end if;
    end process program_pipeline;
    
    --Instruction out to arithmetic units and memory
    i_instr_out : process (clk,rst)
    begin
    if rst = '1' then
        instr_o <= (others => '0');
	elsif rising_edge(clk) then
	    if prog_line_pipe2(25) = '1' then
            instr_o <= (others => '0');
	    else
		    instr_o(24 downto 23) <= prog_line_pipe2(22 downto 21); --Mult control
		    instr_o(16 downto 16) <= prog_line_pipe2(16 downto 16); --Mem write
		    instr_o(7 downto 0)   <= prog_line_pipe2(7 downto 0);   --Addr B

		    --Special address for A (point queue select)
		    if prog_line_pipe2(24) = '1' then
		        instr_o(15 downto 8) <= "1" & std_logic_vector(to_unsigned(queue_pointer,5)) & prog_line_pipe2(9 downto 8);
		    else
	            instr_o(15 downto 8) <= prog_line_pipe2(15 downto 8);
		    end if;
		
		    --Adder controls
            case prog_line_pipe2(19 downto 17) is -- prog_line(20) will trigger the input operands to load
                when "000"  => instr_o(22 downto 17) <= "000000"; -- NOP
                when "001"  => instr_o(22 downto 17) <= "001010"; -- ADD
                when "010"  => instr_o(22 downto 17) <= "001011"; -- SUB
		        when "101"  => instr_o(22 downto 17) <= "110001"; -- Reduction for Fp add (subtraction)
		        when "110"  => instr_o(22 downto 17) <= "110000"; -- Reduction for FP sub (addition)
                when "111"  => instr_o(22 downto 17) <= "110101"; -- Mult Reduction (subtraction
                when others => instr_o(22 downto 17) <= (others => '0');
		    end case;
	    end if;
    end if;
    end process i_instr_out;

end structural;
