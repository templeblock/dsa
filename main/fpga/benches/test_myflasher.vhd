use std.textio.all;
library ieee; use ieee.std_logic_1164.all;


entity myflasher_tb is
end entity;



architecture test1 of myflasher_tb is
	component myflasher is
		port(clk:in std_logic;
			 red:out std_logic;
			 green:out std_logic);
	end component;
	
	constant clk_frequency:real:=133.0; --MHz
	constant clk_period:time:=1.0 us /clk_frequency;
	signal clk:std_logic;
	signal red:std_logic;
	signal green:std_logic;
begin
	myflasher_0:myflasher
		port map(clk=>clk,red=>red, green=>green);
	
	
	clk_process: process
	begin
		clk<='0';
		wait for clk_period/2;
		clk<='1';
		wait for clk_period/2;
	end process;
	
	
	process
		variable l:line;
	begin
		write(l,String'("Hello World"));
		writeline(output,l);
		wait;
	end process;
end test1;


