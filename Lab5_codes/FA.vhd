
library ieee;
use ieee.std_logic_1164.all;

entity FA is
	port(
	a, b, c: in std_logic;
	sum, carry: out std_logic
	);
end FA;

architecture FA_stru of FA is
begin
	sum <= (a xor b) xor c;
	carry <= b when ((a xor b) = '0') else c;
end FA_stru;




