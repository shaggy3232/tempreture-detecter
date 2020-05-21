library ieee;
use ieee.std_logic_1164.all;

entity RCA is
	port(
		A : in std_logic_vector(7 downto 0);
		B : in std_logic_vector(7 downto 0);
		cin : in std_logic;
		sum : out std_logic_vector(7 downto 0);
		flags: out std_logic_vector(3 downto 0)
	);
end RCA;

architecture stru of RCA is
	signal carry: std_logic_vector(6 downto 0);
	signal sumTemp: std_logic_vector(7 downto 0);
	signal cout: std_logic;
	
	component FA
		port(a, b, c: in std_logic;
			sum, carry: out std_logic
		);
	end component;
begin
	FA0: FA port map(A(0), B(0), cin, sumTemp(0), carry(0));
	FA1: FA port map(A(1), B(1), carry(0), sumTemp(1), carry(1));
	FA2: FA port map(A(2), B(2), carry(1), sumTemp(2), carry(2));
	FA3: FA port map(A(3), B(3), carry(2), sumTemp(3), carry(3));
	FA4: FA port map(A(4), B(4), carry(3), sumTemp(4), carry(4));
	FA5: FA port map(A(5), B(5), carry(4), sumTemp(5), carry(5));
	FA6: FA port map(A(6), B(6), carry(5), sumTemp(6), carry(6));
	FA7: FA port map(A(7), B(7), carry(6), sumTemp(7), cout);
	
	sum <= sumTemp;
	
	-- Flags
	-- Complete the expressions for the flags.
	flags(0) <= ;		-- carry out
	flags(1) <= ; 		-- overflow flags
	flags(2) <= ; 		-- negative flags
	flags(3) <= ; 		-- zero flag
end stru;




