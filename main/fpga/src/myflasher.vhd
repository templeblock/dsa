library ieee;	use ieee.std_logic_1164.all;


entity myflasher is
	port(clk:in std_logic;
	     red:out std_logic;
	     green:out std_logic);
end entity;


architecture arch1 of myflasher is
	signal counter:integer range 0 to 10000000;
	signal toggle:std_logic:='0';
begin
	
	counter_process: process(clk)
	begin
		if rising_edge(clk) then
			if counter=10000000 then
				counter<=0;
				toggle<=not toggle;
			else
				counter<=counter+1;
			end if;
		end if;
	end process;
	
	red<='1';
	green<=toggle;
end arch1;

