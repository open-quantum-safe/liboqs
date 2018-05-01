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


entity keccak_round is

port (

    round_in     : in  k_state;
    round_constant_signal    : in std_logic_vector(63 downto 0);
    round_out    : out k_state);

end keccak_round;

architecture rtl of keccak_round is


  ----------------------------------------------------------------------------
  -- Internal signal declarations
  ----------------------------------------------------------------------------

 
  signal theta_in,theta_out,pi_in,pi_out,rho_in,rho_out,chi_in,chi_out,iota_in,iota_out : k_state;
  signal sum_sheet: k_plane;
 
  
begin  -- Rtl




--connecitons

--order theta, pi, rho, chi, iota
theta_in<=round_in;
pi_in<=rho_out;
rho_in<=theta_out;
chi_in<=pi_out;
iota_in<=chi_out;
round_out<=iota_out;


--chi
i0000: for y in 0 to 4 generate
	i0001: for x in 0 to 2 generate
		i0002: for i in 0 to 63 generate
			chi_out(y)(x)(i)<=chi_in(y)(x)(i) xor  ( not(chi_in (y)(x+1)(i))and chi_in (y)(x+2)(i));
		
		end generate;	
	end generate;
end generate;

	i0011: for y in 0 to 4 generate
		i0021: for i in 0 to 63 generate
			chi_out(y)(3)(i)<=chi_in(y)(3)(i) xor  ( not(chi_in (y)(4)(i))and chi_in (y)(0)(i));
		
		end generate;	
	end generate;
	
	i0012: for y in 0 to 4 generate
		i0022: for i in 0 to 63 generate
			chi_out(y)(4)(i)<=chi_in(y)(4)(i) xor  ( not(chi_in (y)(0)(i))and chi_in (y)(1)(i));
		
		end generate;	
	end generate;


--theta

--compute sum of columns

i0101: for x in 0 to 4 generate
	i0102: for i in 0 to 63 generate
		sum_sheet(x)(i)<=theta_in(0)(x)(i) xor theta_in(1)(x)(i) xor theta_in(2)(x)(i) xor theta_in(3)(x)(i) xor theta_in(4)(x)(i);
	
	end generate;	
end generate;


i0200: for y in 0 to 4 generate
	i0201: for x in 1 to 3 generate
		theta_out(y)(x)(0)<=theta_in(y)(x)(0) xor sum_sheet(x-1)(0) xor sum_sheet(x+1)(63);
		i0202: for i in 1 to 63 generate
			theta_out(y)(x)(i)<=theta_in(y)(x)(i) xor sum_sheet(x-1)(i) xor sum_sheet(x+1)(i-1);
		end generate;	
	end generate;
end generate;

i2001: for y in 0 to 4 generate
	theta_out(y)(0)(0)<=theta_in(y)(0)(0) xor sum_sheet(4)(0) xor sum_sheet(1)(63);
	i2021: for i in 1 to 63 generate
		theta_out(y)(0)(i)<=theta_in(y)(0)(i) xor sum_sheet(4)(i) xor sum_sheet(1)(i-1);
	end generate;	

end generate;

i2002: for y in 0 to 4 generate
	theta_out(y)(4)(0)<=theta_in(y)(4)(0) xor sum_sheet(3)(0) xor sum_sheet(0)(63);
	i2022: for i in 1 to 63 generate
		theta_out(y)(4)(i)<=theta_in(y)(4)(i) xor sum_sheet(3)(i) xor sum_sheet(0)(i-1);
	end generate;	

end generate;

-- pi
i3001: for y in 0 to 4 generate
	i3002: for x in 0 to 4 generate
		i3003: for i in 0 to 63 generate
			--pi_out(y)(x)(i)<=pi_in((y +2*x) mod 5)(((4*y)+x) mod 5)(i);
			pi_out((2*x+3*y) mod 5)(0*x+1*y)(i)<=pi_in(y) (x)(i);
		end generate;	
	end generate;
end generate;

--rho


i4001: for i in 0 to 63 generate
	rho_out(0)(0)(i)<=rho_in(0)(0)(i);
end generate;	
i4002: for i in 0 to 63 generate
	rho_out(0)(1)(i)<=rho_in(0)(1)((i-1)mod 64);
end generate;
i4003: for i in 0 to 63 generate
	rho_out(0)(2)(i)<=rho_in(0)(2)((i-62)mod 64);
end generate;
i4004: for i in 0 to 63 generate
	rho_out(0)(3)(i)<=rho_in(0)(3)((i-28)mod 64);
end generate;
i4005: for i in 0 to 63 generate
	rho_out(0)(4)(i)<=rho_in(0)(4)((i-27)mod 64);
end generate;

i4011: for i in 0 to 63 generate
	rho_out(1)(0)(i)<=rho_in(1)(0)((i-36)mod 64);
end generate;	
i4012: for i in 0 to 63 generate
	rho_out(1)(1)(i)<=rho_in(1)(1)((i-44)mod 64);
end generate;
i4013: for i in 0 to 63 generate
	rho_out(1)(2)(i)<=rho_in(1)(2)((i-6)mod 64);
end generate;
i4014: for i in 0 to 63 generate
	rho_out(1)(3)(i)<=rho_in(1)(3)((i-55)mod 64);
end generate;
i4015: for i in 0 to 63 generate
	rho_out(1)(4)(i)<=rho_in(1)(4)((i-20)mod 64);
end generate;

i4021: for i in 0 to 63 generate
	rho_out(2)(0)(i)<=rho_in(2)(0)((i-3)mod 64);
end generate;	
i4022: for i in 0 to 63 generate
	rho_out(2)(1)(i)<=rho_in(2)(1)((i-10)mod 64);
end generate;
i4023: for i in 0 to 63 generate
	rho_out(2)(2)(i)<=rho_in(2)(2)((i-43)mod 64);
end generate;
i4024: for i in 0 to 63 generate
	rho_out(2)(3)(i)<=rho_in(2)(3)((i-25)mod 64);
end generate;
i4025: for i in 0 to 63 generate
	rho_out(2)(4)(i)<=rho_in(2)(4)((i-39)mod 64);
end generate;

i4031: for i in 0 to 63 generate
	rho_out(3)(0)(i)<=rho_in(3)(0)((i-41)mod 64);
end generate;	
i4032: for i in 0 to 63 generate
	rho_out(3)(1)(i)<=rho_in(3)(1)((i-45)mod 64);
end generate;
i4033: for i in 0 to 63 generate
	rho_out(3)(2)(i)<=rho_in(3)(2)((i-15)mod 64);
end generate;
i4034: for i in 0 to 63 generate
	rho_out(3)(3)(i)<=rho_in(3)(3)((i-21)mod 64);
end generate;
i4035: for i in 0 to 63 generate
	rho_out(3)(4)(i)<=rho_in(3)(4)((i-8)mod 64);
end generate;

i4041: for i in 0 to 63 generate
	rho_out(4)(0)(i)<=rho_in(4)(0)((i-18)mod 64);
end generate;	
i4042: for i in 0 to 63 generate
	rho_out(4)(1)(i)<=rho_in(4)(1)((i-2)mod 64);
end generate;
i4043: for i in 0 to 63 generate
	rho_out(4)(2)(i)<=rho_in(4)(2)((i-61)mod 64);
end generate;
i4044: for i in 0 to 63 generate
	rho_out(4)(3)(i)<=rho_in(4)(3)((i-56)mod 64);
end generate;
i4045: for i in 0 to 63 generate
	rho_out(4)(4)(i)<=rho_in(4)(4)((i-14)mod 64);
end generate;

--iota
i5001: for y in 1 to 4 generate
	i5002: for x in 0 to 4 generate
		i5003: for i in 0 to 63 generate
			iota_out(y)(x)(i)<=iota_in(y)(x)(i);
		end generate;	
	end generate;
end generate;


	i5012: for x in 1 to 4 generate
		i5013: for i in 0 to 63 generate
			iota_out(0)(x)(i)<=iota_in(0)(x)(i);
		end generate;	
	end generate;



		i5103: for i in 0 to 63 generate
			iota_out(0)(0)(i)<=iota_in(0)(0)(i) xor round_constant_signal(i);
		end generate;	



end rtl;
