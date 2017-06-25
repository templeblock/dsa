library ieee; use ieee.std_logic_1164.all;

entity wb_interface_tb is
end entity;

use work.spi_pkg.wb_interface;
architecture bench1 of wb_interface_tb is
	constant clk_frequency:real:=88.67; --MHz
	constant clk_period:time:=1.0 us /clk_frequency;
	signal clk:std_logic;
	
	signal wbcyc  :std_logic;
	signal wbstb  :std_logic;
	signal wbwe   :std_logic;
	signal wbaddr :std_logic_vector(7 downto 0);
	signal wbdat_i:std_logic_vector(7 downto 0);
	signal wbdat_o:std_logic_vector(7 downto 0);
	signal wback  :std_logic:='0';
	
	signal write_data:std_logic_vector(7 downto 0);
	signal writereq :std_logic:='0';
	signal new_data:std_logic;
begin
	uut:wb_interface
		port map(clk=>clk,
		         wbcyc=>wbcyc,
		         wbstb=>wbstb,
		         wbwe =>wbwe,
		         wbaddr=>wbaddr,
		         wbdat_i=>wbdat_i,
		         wbdat_o=>wbdat_o,
		         wback  =>wback,
		         read_data=>open, new_data=>new_data,
		         write_data=>write_data, write_req=>writereq);
	
	clk_process: process
	begin
		clk<='0';
		wait for clk_period/2;
		clk<='1';
		wait for clk_period/2;
	end process;
	
	main: process
		variable toggle:std_logic:='0';
	begin
		wait until wbcyc='1';
		wait for clk_period*1.75;
		wback<='1';
		if wbaddr=x"56" then
			assert wbdat_i=x"00" report "NOT USING SPI SLAVE MODE";
		elsif wbaddr=x"5a" then --SPISR
			assert wbwe='0' report "TRYING TO WRITE TO SPISR";
			wbdat_o<="00000000";
			wbdat_o(7)<=toggle;	wbdat_o(3)<=toggle; wbdat_o(4)<=toggle;
			toggle:=not toggle;
		elsif wbaddr=x"5b" then --SPIRXDR
			wbdat_o<=x"65";
		end if;
		
		wait for clk_period/2;
		wback<='0';
		wbdat_o<="UUUUUUUU";
	end process;
	
	write: process
	begin
		wait for 50 ns;
		write_data<=x"42";
		writereq<='1';
		wait for clk_period;
		writereq<='0';
		
		wait until new_data='1';
		wait for 150 ns;
		
		write_data<=x"77";
		writereq<='1';
		wait for clk_period;
		writereq<='0';
		
		wait until new_data='1';
	end process;

end bench1;

