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

entity sike_arith_unit is
    Port ( clk                     : in  std_logic;
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
end sike_arith_unit; 

architecture structural of sike_arith_unit is
    component fau_752 is
        Port ( clk        : in  std_logic;
               rst        : in  std_logic;
               mult_ctrl_i: in  std_logic_vector(1 downto 0);
               add_ctrl_i : in  std_logic_vector(5 downto 0);
               opa_i      : in  std_logic_vector(751 downto 0);
               opb_i      : in  std_logic_vector(751 downto 0);
               res_o      : out std_logic_vector(751 downto 0)
               );    
    end component fau_752;

    component reg_file_752 IS
      PORT ( clka  : IN  std_logic;
             ena   : IN  std_logic;
             wea   : IN  std_logic_vector(0 DOWNTO 0);
             addra : IN  std_logic_vector(7 DOWNTO 0);
             dina  : IN  std_logic_vector(751 DOWNTO 0);
             douta : OUT std_logic_vector(751 DOWNTO 0);
             clkb  : IN  std_logic;
             enb   : IN  std_logic;
             web   : IN  std_logic_vector(0 DOWNTO 0);
             addrb : IN  std_logic_vector(7 DOWNTO 0);
             dinb  : IN  std_logic_vector(751 DOWNTO 0);
             doutb : OUT std_logic_vector(751 DOWNTO 0)
             );
    end component reg_file_752;
    
    component iso_ctrl is
        Port ( clk         : in  std_logic;
               rst         : in  std_logic;
               iso_cmd_i   : in  std_logic_vector (2 downto 0);
               digit_in    : in  std_logic;
	       instr_o     : out std_logic_vector(24 downto 0);
               req_digit_o : out std_logic;
               alice_o     : out std_logic;
               busy_o      : out std_logic
               );
    end component iso_ctrl;  

    component keccak_1088 is
  
    port (
        clk           : in  std_logic;
        rst           : in  std_logic;
        clear_state   : in  std_logic;
        din           : in  std_logic_vector(7 downto 0);
        din_valid     : in  std_logic;
        buffer_full   : out std_logic;
        ready         : out std_logic;
        dout          : out std_logic_vector(511 downto 0));
    end component keccak_1088;


    -- Memory interface
    signal reg_dinb : std_logic_vector(751 downto 0);
    signal reg_addr_r, reg_addrb : std_logic_vector(7 downto 0);
    signal reg_dout_r : std_logic_vector(751 downto 0);
    signal reg_web_r : std_logic;

    signal di_mux : std_logic_vector(63 downto 0);
    signal do : std_logic_vector(63 downto 0);
    -- FAU signals
    signal opa, opb : std_logic_vector(751 downto 0);
    signal res : std_logic_vector(751 downto 0);
    
    signal iso_instr_s : std_logic_vector(24 downto 0);

    signal alice_s : std_logic;
    signal Alicekey : std_logic_vector(371 downto 0);
    signal Bobkey   : std_logic_vector(378 downto 0);
    signal keys_r : std_logic_vector(767 downto 0);
   
    signal iso_busy_s : std_logic;
    
    signal digit_in : std_logic;
    signal req_digit : std_logic;
 
    signal alice_secret_msg : std_logic_vector(255 downto 0);
    signal bob_secret_msg : std_logic_vector(255 downto 0);
    signal bob_cipher_msg : std_logic_vector(255 downto 0);

    signal msg_buffer_r    : std_logic_vector(767 downto 0);
 
    signal keccak_working_s : std_logic;
    signal keccak_working_r : std_logic;
    signal keccak_cnt_r : std_logic_vector(2 downto 0);
    signal keccak_buffer_in_r : std_logic_vector(63 downto 0);
    signal keccak_din_s : std_logic_vector(7 downto 0);
    signal keccak_din_valid_s : std_logic;
    signal keccak_buffer_full_s : std_logic;
    signal keccak_ready_s : std_logic;
    signal keccak_buffer_out_s : std_logic_vector(511 downto 0);
    signal keccak_busy_s : std_logic;
    signal w_one_3      : std_logic_vector(2 downto 0);
    
    signal shared_secret : std_logic_vector(191 downto 0);

begin
    shared_secret <= keccak_buffer_out_s(191 downto 0);
    w_one_3 <= "001";
    alice_secret_msg <= msg_buffer_r(767 downto 512);
    bob_cipher_msg   <= msg_buffer_r(511 downto 256);
    bob_secret_msg   <= msg_buffer_r(255 downto 0);
    --Zeroize unused key bits
    Alicekey <= keys_r(755 downto 384);
    Bobkey <= keys_r(378 downto 0);
    keys_r(767 downto 756) <= (others => '0');
    keys_r(383 downto 379)   <= (others => '0');
    di_mux <= data_i when (wr_input_sel_i='0') else
              do;
    keccak_working_s <= '1' when ((keccak_word_valid_i = '1') or (keccak_cnt_r /= "000")) else
                        '0';
    keccak_din_s <= keccak_din_i when keccak_din_valid_byte_i = '1' else
                    keccak_buffer_in_r(7 downto 0);
    keccak_din_valid_s <= (keccak_working_r or keccak_din_valid_byte_i) and (not keccak_buffer_full_s);
    -- Isogeny Control logic
    i_iso_ctrl : iso_ctrl
        port map (clk       => clk,
	          rst       => rst,
		  iso_cmd_i => iso_cmd_i,
		  digit_in  => digit_in,
		  instr_o   => iso_instr_s,
                  req_digit_o => req_digit,
		  alice_o   => alice_s,
		  busy_o    => iso_busy_s);
		
    -- Register file memory
    i_reg_file : reg_file_752
        port map (clka   => clk,
                  ena    => '1',
                  wea(0) => iso_instr_s(16),
                  addra  => iso_instr_s(15 downto 8),
                  dina   => res,
                  douta  => opa,
                  clkb   => clk,
                  enb    => '1',
                  web(0) => reg_web_r,
                  addrb  => reg_addrb,
                  dinb   => reg_dinb,
                  doutb  => opb);

    -- Field arithmetic unit
    i_fau : fau_752
        port map (clk         => clk,
                  rst         => rst,
                  mult_ctrl_i => iso_instr_s(24 downto 23),
                  add_ctrl_i  => iso_instr_s(22 downto 17),
                  opa_i       => opa,
                  opb_i       => opb,
                  res_o       => res);   

    --Keccak block
    i_keccak : keccak_1088
        port map(clk         => clk, 
                 rst         => rst, 
                 clear_state => keccak_clear_i, 
                 din         => keccak_din_s, 
                 din_valid   => keccak_din_valid_s, 
                 buffer_full => keccak_buffer_full_s, 
                 ready       => keccak_ready_s, 
                 dout        => keccak_buffer_out_s); 


    -- Memory mapped register interface
    i_reg_interface : process (clk,rst)
    begin
        -- Asynchronous active-high reset
        if rst = '1' then
        
            reg_dinb               <= (others => '0');
            reg_addr_r             <= (others => '0');
            reg_web_r              <= '0';
            keys_r(755 downto 384) <= (others => '0');
            keys_r(378 downto 0)   <= (others => '0');
            digit_in               <= '0';
            msg_buffer_r           <= (others => '0');
            keccak_cnt_r           <= (others => '0');
            keccak_buffer_in_r     <= (others => '0');
            keccak_working_r       <= '0';
            reg_dout_r             <= (others => '0');
        
        elsif rising_edge(clk) then

            -- Write new register
            if (wr_en_i = '1') and (wr_op_sel_i = "00") then
                case wr_word_sel_i is
                    when "0000" => reg_dinb(63 downto 0)    <= di_mux;
                    when "0001" => reg_dinb(127 downto 64)  <= di_mux;
                    when "0010" => reg_dinb(191 downto 128) <= di_mux;
                    when "0011" => reg_dinb(255 downto 192) <= di_mux;
                    when "0100" => reg_dinb(319 downto 256) <= di_mux;
                    when "0101" => reg_dinb(383 downto 320) <= di_mux;
                    when "0110" => reg_dinb(447 downto 384) <= di_mux;
                    when "0111" => reg_dinb(511 downto 448) <= di_mux;
                    when "1000" => reg_dinb(575 downto 512) <= di_mux;
                    when "1001" => reg_dinb(639 downto 576) <= di_mux;
                    when "1010" => reg_dinb(703 downto 640) <= di_mux;
                    when "1011" => reg_dinb(751 downto 704) <= di_mux(47 downto 0);
                    when others =>
                end case;
            end if;

            -- Write new key
            if (wr_en_i = '1') and (wr_op_sel_i = "01") then
                case wr_word_sel_i is
                    when "0000" => keys_r(63 downto 0)    <= di_mux;
                    when "0001" => keys_r(127 downto 64)  <= di_mux;
                    when "0010" => keys_r(191 downto 128) <= di_mux;
                    when "0011" => keys_r(255 downto 192) <= di_mux; 
                    when "0100" => keys_r(319 downto 256) <= di_mux;
                    when "0101" => keys_r(378 downto 320) <= di_mux(58 downto 0);
                    when "0110" => keys_r(447 downto 384) <= di_mux;
                    when "0111" => keys_r(511 downto 448) <= di_mux;
                    when "1000" => keys_r(575 downto 512) <= di_mux;
                    when "1001" => keys_r(639 downto 576) <= di_mux;
                    when "1010" => keys_r(703 downto 640) <= di_mux;
                    when "1011" => keys_r(755 downto 704) <= di_mux(51 downto 0);
                    when others =>
                end case;
            elsif req_digit = '1' then
                --Shift digit
                if alice_s = '1' then
                    keys_r(754 downto 384) <= keys_r(755 downto 385);
                    keys_r(755) <= keys_r(384);
                else
                    keys_r(377 downto 0) <= keys_r(378 downto 1);
                    keys_r(378) <= keys_r(0);
                end if;
            end if;
            -- Message buffer logic
            if (wr_en_i = '1') and (wr_op_sel_i = "10") then
                case wr_word_sel_i is
                    when "0000" => msg_buffer_r(63 downto 0)    <= di_mux;
                    when "0001" => msg_buffer_r(127 downto 64)  <= di_mux;
                    when "0010" => msg_buffer_r(191 downto 128) <= di_mux;
                    when "0011" => msg_buffer_r(255 downto 192) <= di_mux;
                    when "0100" => msg_buffer_r(319 downto 256) <= di_mux;
                    when "0101" => msg_buffer_r(383 downto 320) <= di_mux;
                    when "0110" => msg_buffer_r(447 downto 384) <= di_mux;
                    when "0111" => msg_buffer_r(511 downto 448) <= di_mux;
                    when "1000" => msg_buffer_r(575 downto 512) <= di_mux;
                    when "1001" => msg_buffer_r(639 downto 576) <= di_mux;
                    when "1010" => msg_buffer_r(703 downto 640) <= di_mux;
                    when "1011" => msg_buffer_r(767 downto 704) <= di_mux;
                    when others => 
                end case;
            elsif buffer_xor_i = '1' then
                msg_buffer_r(511 downto 256) <= msg_buffer_r(255 downto 0) xor keccak_buffer_out_s(255 downto 0);
            end if;
                -- Keccak word in
            keccak_working_r <= keccak_working_s;
            if (keccak_word_valid_i = '1') then
                keccak_cnt_r <= keccak_word_cnt_i;
            elsif (keccak_working_r = '1') and (keccak_cnt_r /= "000") and (keccak_buffer_full_s = '0') then
                keccak_cnt_r <= std_logic_vector(unsigned(keccak_cnt_r) - unsigned(w_one_3));
            end if;

            if (wr_en_i = '1') and (wr_op_sel_i = "11") then
                keccak_buffer_in_r <= di_mux;
            elsif ((keccak_working_r = '1') and (keccak_buffer_full_s = '0')) then
                keccak_buffer_in_r(63 downto 56) <= keccak_buffer_in_r(7 downto 0);
                keccak_buffer_in_r(55 downto 0)  <= keccak_buffer_in_r(63 downto 8);
            end if;

            --Push key bit to iso control
            if alice_s = '1' then
                digit_in <= keys_r(384);
            else
                digit_in <= keys_r(0);
            end if;
                        
            reg_addr_r <= reg_sel_i;
            
            if (wr_op_sel_i = "00") then
                reg_web_r <= wr_en_i and wr_word_sel_i(0) and wr_word_sel_i(1) and (not wr_word_sel_i(2)) and wr_word_sel_i(3); -- Write after the highest 64-bit word has been written
            end if;
            -- Read multiplexer    
            if (rd_reg_i = '1') then
                reg_dout_r <= opb;
            end if;
        end if;
    end process;
    -- Memory mapped register interface
    i_read_process : process (rd_op_sel_i, rd_word_sel_i, reg_dout_r,keys_r,msg_buffer_r,keccak_buffer_out_s)
    begin
        case rd_op_sel_i is
            when "00" =>
                case rd_word_sel_i is
                    when "0000" => do               <= reg_dout_r(63 downto 0) ;
                    when "0001" => do               <= reg_dout_r(127 downto 64);
                    when "0010" => do               <= reg_dout_r(191 downto 128);
                    when "0011" => do               <= reg_dout_r(255 downto 192);
                    when "0100" => do               <= reg_dout_r(319 downto 256);
                    when "0101" => do               <= reg_dout_r(383 downto 320);
                    when "0110" => do               <= reg_dout_r(447 downto 384);
                    when "0111" => do               <= reg_dout_r(511 downto 448);
                    when "1000" => do               <= reg_dout_r(575 downto 512);
                    when "1001" => do               <= reg_dout_r(639 downto 576);
                    when "1010" => do               <= reg_dout_r(703 downto 640);
                    when "1011" => do(47 downto 0)  <= reg_dout_r(751 downto 704);
                                   do(63 downto 48) <= (others => '0');
                    when others => do               <= (others => '0');
                end case;            
            when "01" =>
                case rd_word_sel_i is
                    when "0000" => do               <= keys_r(63 downto 0) ;
                    when "0001" => do               <= keys_r(127 downto 64);
                    when "0010" => do               <= keys_r(191 downto 128);
                    when "0011" => do               <= keys_r(255 downto 192);
                    when "0100" => do               <= keys_r(319 downto 256);
                    when "0101" => do               <= keys_r(383 downto 320);
                    when "0110" => do               <= keys_r(447 downto 384);
                    when "0111" => do               <= keys_r(511 downto 448);
                    when "1000" => do               <= keys_r(575 downto 512);
                    when "1001" => do               <= keys_r(639 downto 576);
                    when "1010" => do               <= keys_r(703 downto 640);
                    when "1011" => do               <= keys_r(767 downto 704);
                    when others => do               <= (others => '0');
                end case;            
            when "10" =>
                case rd_word_sel_i is
                    when "0000" => do               <= msg_buffer_r(63 downto 0) ;
                    when "0001" => do               <= msg_buffer_r(127 downto 64);
                    when "0010" => do               <= msg_buffer_r(191 downto 128);
                    when "0011" => do               <= msg_buffer_r(255 downto 192);
                    when "0100" => do               <= msg_buffer_r(319 downto 256);
                    when "0101" => do               <= msg_buffer_r(383 downto 320);
                    when "0110" => do               <= msg_buffer_r(447 downto 384);
                    when "0111" => do               <= msg_buffer_r(511 downto 448);
                    when "1000" => do               <= msg_buffer_r(575 downto 512);
                    when "1001" => do               <= msg_buffer_r(639 downto 576);
                    when "1010" => do               <= msg_buffer_r(703 downto 640);
                    when "1011" => do               <= msg_buffer_r(767 downto 704);
                    when others => do               <= (others => '0');
                end case;            
            when "11" =>
                case rd_word_sel_i is
                    when "0000" => do               <= keccak_buffer_out_s(63 downto 0) ;
                    when "0001" => do               <= keccak_buffer_out_s(127 downto 64);
                    when "0010" => do               <= keccak_buffer_out_s(191 downto 128);
                    when "0011" => do               <= keccak_buffer_out_s(255 downto 192);
                    when "0100" => do               <= keccak_buffer_out_s(319 downto 256);
                    when "0101" => do               <= keccak_buffer_out_s(383 downto 320);
                    when "0110" => do               <= keccak_buffer_out_s(447 downto 384);
                    when "0111" => do               <= keccak_buffer_out_s(511 downto 448);
                    when others => do               <= (others => '0');
                end case;            
            when others =>
        end case;   
    end process;         
            
    reg_addrb <= iso_instr_s(7 downto 0) when iso_busy_s = '1' else reg_addr_r;
    keccak_busy_s <= keccak_buffer_full_s or keccak_working_s;
    busy_o <= iso_busy_s or keccak_busy_s;
    data_o <= do;

end structural;
