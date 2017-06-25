library IEEE;       use IEEE.std_logic_1164.all;
                    use ieee.numeric_std.all;
library machxo2;    use machxo2.components.all;

entity main is
	port(	GSRn:in  std_logic;
			LEDR:out std_logic;
			LEDG:out std_logic;
			
			SCLK:inout std_logic;
			MISO:inout std_logic;
			MOSI:inout std_logic;
			CE1 :in    std_logic;
			
			GPIO22:in  std_logic; --parallel data (8 bits)
			GPIO23:in  std_logic;
			GPIO24:in  std_logic;
			GPIO25:in  std_logic;
			GPIO14:in  std_logic;
			GPIO15:in  std_logic;
			GPIO17:in  std_logic;
			GPIO18:in  std_logic;
			GPIO27:in  std_logic; --parallel clk
			SDA   :in  std_logic);--parallel reset
end main;


use work.spi_pkg.spi;
architecture arch1 of main is
--##################COMPONENTS#########################
	
	--##LATTICE OSCILLATOR PRIMITIVE##
	component osch
		generic (nom_freq:string:="2.08");
		port(stdby:in std_logic;
		     osc:out std_logic;
			 sedstdby:out std_logic);
	end component;


--#####################################################
	signal GSRnX        : std_logic;
	attribute pullmode  : string;
	attribute pullmode of GSRnX: signal is "UP";  -- else floats
	
	signal clk:std_logic;
	
	signal spi_data:std_logic_vector(7 downto 0):=x"77";
	signal new_data:std_logic;
	signal write_data:std_logic_vector(7 downto 0):=x"ff";
	signal write_req:std_logic:='0';
	
	signal parallel_data:unsigned(7 downto 0);
	signal parallel_clk :std_logic;
	signal parallel_rst :std_logic;
	signal checksum:unsigned(15 downto 0):=to_unsigned(0,16);
begin
	-- global reset--------------------------------
	IBgsr   : IB  port map ( I=>GSRn, O=>GSRnX );
	GSR_GSR : GSR port map ( GSR=>GSRnX );
	-----------------------------------------------
	
	osc_0:osch
		generic map(nom_freq=>"88.67")
		port    map(stdby=>'0', osc=>clk, sedstdby=>open);
	
	
		
	spi_0:spi
		port map(clk=>clk,spi_clk=>SCLK,
		         spi_miso=>MISO,spi_mosi=>MOSI,
		         spi_cs=>CE1,read_data=>spi_data,new_data=>new_data,
		         write_data=>write_data, write_req=>write_req);
	
	
	process(clk,SDA)
		variable bytenumber:integer range 0 to 3:=0;
	begin
	if SDA='1' then
		bytenumber:=0;
	elsif rising_edge(clk) then
		if new_data='1' then
			case bytenumber is
			when 0 =>
				write_data<=std_logic_vector(checksum(7 downto 0));
				bytenumber:=bytenumber+1;
				write_req<='1';
			when 1 =>
				write_data<=std_logic_vector(checksum(15 downto 8));
				bytenumber:=bytenumber+1;
				write_req<='1';
			when others =>
				write_data<=x"ff";
				--bytenumber:=0;
			end case;
			
		else
			write_req<='0';
		end if;
	end if;
	end process;
	
	
	
	--parallel input-------------------------------
	parallel_data(0)<=GPIO18; --not working
	parallel_data(1)<=GPIO17;
	parallel_data(2)<=GPIO14;
	parallel_data(3)<=GPIO15;
	parallel_data(4)<=GPIO22;
	parallel_data(5)<=GPIO23;
	parallel_data(6)<=GPIO24;
	parallel_data(7)<=GPIO25;
	parallel_clk<=GPIO27;
	parallel_rst<=SDA;
	
	
	process(parallel_clk,parallel_rst)
	begin
	if parallel_rst='1' then
		checksum<=to_unsigned(0,checksum'length);
	elsif rising_edge(parallel_clk) then
		checksum<=checksum+parallel_data;
	end if;
	end process;
	
	
	--FIXME: this process is incorrect (but would be easier to sync with the rest)
--	process(clk,parallel_clk,SDA)
--		variable new_clock:std_logic:='0';
--	begin
--		if parallel_rst='1' then
--			checksum<=to_unsigned(0,checksum'length);
--		elsif parallel_clk='0' then
--			new_clock:='1';
--		elsif rising_edge(clk) then
--			if new_clock='1' then
--				new_clock:='0';
--
--				checksum<=checksum+parallel_data;
--			end if;
--		end if;
--	end process;
	
	
	
	--LEDs-----------------------------------------
	LEDR<=GPIO17;
	LEDG<=GPIO18; --gpio18 not working :[
	-----------------------------------------------
end arch1;
