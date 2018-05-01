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

entity fau_752 is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           mult_ctrl_i: in STD_LOGIC_VECTOR (1 downto 0);
           add_ctrl_i : in STD_LOGIC_VECTOR(5 downto 0);
           opa_i : in STD_LOGIC_VECTOR (751 downto 0);
           opb_i : in STD_LOGIC_VECTOR (751 downto 0);
           res_o : out STD_LOGIC_VECTOR (751 downto 0));
end fau_752;

architecture rtl of fau_752 is

component add_sub_752 is
    port (  clk   : in std_logic;
            rst   : in std_logic;
            opa_i : in std_logic_vector(751 downto 0);
            opb_i : in std_logic_vector(751 downto 0);
            sub_i : in std_logic;
            res_o : out std_logic_vector(751 downto 0);
            neg_o : out std_logic;
            zero_o: out std_logic);
end component add_sub_752;

component mult_unit_752 is
    port (clk              : in std_logic;
          rst              : in std_logic;
          start_i          : in std_logic;
          opa_i            : in std_logic_vector(751 downto 0);
          opb_i            : in std_logic_vector(751 downto 0);
          res_read_i       : in std_logic;
          reset_even_odd_i : in std_logic;
          res_o            : out std_logic_vector(751 downto 0));        
end component mult_unit_752;

    signal prime : std_logic_vector(751 downto 0);
    -- Signals for full-precision multiplication
    signal mult_start_s : std_logic;
    signal mult_reset_even_odd_s : std_logic;
    signal mult_opa, mult_opb : std_logic_vector(751 downto 0);
    signal mult_res : std_logic_vector(751 downto 0);

    signal add_sel_s, add_red_s : std_logic;
    signal add_opa_s, add_opb_s : std_logic_vector(1 downto 0);
    signal add_sel_d1, add_sel_d2, add_sel_d3 : std_logic;
    signal add_red_d1, add_red_d2, add_red_d3 : std_logic;
    
    signal add_a_s, add_b_s, add_res_s : std_logic_vector(751 downto 0);
    signal add_a_d1, add_a_d2, add_a_d3 : std_logic_vector(751 downto 0);
    
    signal res_select_s : std_logic;
    signal add_neg_s: std_logic;
    signal add_neg_d1, add_neg_d2, add_neg_d3: std_logic;
    signal add_zero_s: std_logic;
    signal add_zero_d1, add_zero_d2, add_zero_d3: std_logic;

begin
    prime <= x"6fe5d541f71c0e12909f97badc668562b5045cb25748084e9867d6ebe876da959b1a13f7cc76e3ec968549f878a8eeafffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    
----------------MULTIPILER------------------
    mult_start_s          <= mult_ctrl_i(0);
    mult_reset_even_odd_s <= mult_ctrl_i(1);
    mult_opa <= opa_i;
    mult_opb <= opb_i;

    -- Multiply unit has x multipliers
    i_MULT_UNIT : mult_unit_752
        port map (clk              => clk,
	          rst              => rst, 
		  start_i          => mult_start_s,
		  opa_i            => mult_opa, 
		  opb_i            => mult_opb,
		  res_read_i       => add_opa_s(1),
		  reset_even_odd_i => mult_reset_even_odd_s,
		  res_o            => mult_res);
    
----------------ADDER/SUBTRACTOR------------------
    add_sel_s <= add_ctrl_i(0); --Invert add/sub
    add_opa_s <= add_ctrl_i(2 downto 1);
    add_opb_s <= add_ctrl_i(4 downto 3);
    add_red_s <= add_ctrl_i(5);

    -- Set the inputs to the adder/subtractor based on the add instruction
    add_a_s <= add_res_s       when add_opa_s = "00" else --Accumulate TODO
               opa_i           when add_opa_s = "01" else --Memory load
               mult_res        when add_opa_s = "10" --Multiplier load
               else (others => 'X');
    add_b_s <= (others => '0') when add_opb_s = "00" else --Zero
               opb_i           when add_opb_s = "01" else --Memory Load
               prime           when add_opb_s = "10" --Reduction
            else (others => 'X');

    -- Adder/subtractor
    i_ADDER_SUBTRACTOR : add_sub_752
        port map(clk    => clk, 
	         rst    => rst, 
		 opa_i  => add_a_s, 
		 opb_i  => add_b_s, 
		 sub_i  => add_sel_s, 
		 res_o  => add_res_s, 
		 neg_o  => add_neg_s,
	         zero_o => add_zero_s);
                
    -- Instruction pipeline for addition controls
    i_add_ctrl_pipeline : process (clk,rst)
    begin
        if rst = '1' then
	    add_sel_d1 <= '0'; add_sel_d2 <= '0'; add_sel_d3 <= '0';
	    add_red_d1 <= '0'; add_red_d2 <= '0'; add_red_d3 <= '0';
        elsif rising_edge(clk) then
	    add_sel_d1 <= add_sel_s; add_sel_d2 <= add_sel_d1; add_sel_d3 <= add_sel_d2;
	    add_red_d1 <= add_red_s; add_red_d2 <= add_red_d1; add_red_d3 <= add_red_d2;
        end if;
    end process i_add_ctrl_pipeline;
    
    -- Input registers for adder/subtractor 
    i_add_op_regs : process (clk,rst)
    begin
        if rst = '1' then
            add_a_d1  <= (others => '0'); add_a_d2  <= (others => '0'); add_a_d3  <= (others => '0');
        elsif rising_edge(clk) then
            add_a_d1  <= add_a_s; add_a_d2  <= add_a_d1; add_a_d3  <= add_a_d2;
        end if;
    end process i_add_op_regs; 
    
    --Negative and zero result pipeline
    i_neg_zero_pipeline : process (clk,rst)
    begin
        if rst = '1' then
            add_neg_d1  <= '0'; add_neg_d2  <= '0'; add_neg_d3  <= '0';
            add_zero_d1 <= '0'; add_zero_d2 <= '0'; add_zero_d3 <= '0';
        elsif rising_edge(clk) then
            add_neg_d1  <= add_neg_s;  add_neg_d2  <= add_neg_d1;  add_neg_d3  <= add_neg_d2;
            add_zero_d1 <= add_zero_s; add_zero_d2 <= add_zero_d1; add_zero_d3 <= add_zero_d2;
        end if;
    end process;

    --Select correct result when reducing
    res_select_s <= not(add_red_d3 and add_neg_s) when add_sel_d3 = '1' else --REDADD and REDMULT
		  (add_red_d3 and add_neg_d3 and (not add_zero_d3)); --REDSUB
    
    res_mux : process(add_res_s, add_a_d3, res_select_s)
    begin
    case res_select_s is
        when '0' =>
            res_o <= add_a_d3;
        when '1' =>
            res_o <= add_res_s;
        when others =>
    end case;
    end process;

end rtl;
