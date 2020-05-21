-- Copyright (C) 2018  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Intel Program License 
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details.

-- ***************************************************************************
-- This file contains a Vhdl test bench template that is freely editable to   
-- suit user's needs .Comments are provided in each section to help the user  
-- fill out necessary details.                                                
-- ***************************************************************************
-- Generated on "10/27/2018 11:36:05"
                                                            
-- Vhdl Test Bench template for design  :  FSMctrl
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY RCA_tb IS
END RCA_tb;

ARCHITECTURE RCA_tb_arch OF RCA_tb IS
-- constants 
-- constants   
type test_vector is record
        A: std_logic_vector(7 downto 0);
	B: std_logic_vector(7 downto 0);
	cin: std_logic;
	sum: std_logic_vector(7 downto 0);
	flags: std_logic_vector(3 downto 0);
end record;

type test_vector_array is array (natural range <>) of test_vector;
    constant test_vectors : test_vector_array := ( 
          	("00000001", "00000001", '0', "00000010", "0000"),
		("00000010", "00000001", '0', "00000011", "0000")
	);   
                                              
-- signals                                                   
signal A : std_logic_vector(7 downto 0);
signal B : std_logic_vector(7 downto 0);
signal cin : std_logic;
signal sum : std_logic_vector(7 downto 0);
signal flags: std_logic_vector(3 downto 0);

component RCA 
	port(
		A : in std_logic_vector(7 downto 0);
		B : in std_logic_vector(7 downto 0);
		cin : in std_logic;
		sum : out std_logic_vector(7 downto 0);
		flags: out std_logic_vector(3 downto 0)
	);
end component;

BEGIN
	i1 : RCA
	PORT MAP (
		A => A,
		B => B,
		cin => cin,
		sum => sum,
		flags => flags
	);


test : process
	--variable i : natural range 0 to test_vectors'length := 0;
begin
	for i in 0 to 1 loop
		
		A <= test_vectors(i).A;
		B <= test_vectors(i).B;
		cin <= test_vectors(i).cin;

		wait for 100 ps;

		assert(
			(sum = test_vectors(i).sum) and 
			(flags = test_vectors(i).flags)
		)
		report "test_vector " & integer'image(i) & " failed "
			severity error;
		
		wait for 100 ps;
	end loop;
end process test;  
                                                        
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
WAIT;                                                        
END PROCESS always;                                                                
END RCA_tb_arch;
