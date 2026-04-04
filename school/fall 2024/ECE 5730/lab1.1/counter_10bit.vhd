library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity counter_10bit is
	port (
		ADC_CLK_10 : IN STD_LOGIC;
		MAX10_CLK1_50 : IN STD_LOGIC;
		MAX10_CLK2_50 : IN STD_LOGIC;
		KEY : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
		LEDR : OUT STD_LOGIC_VECTOR(9 DOWNTO 0)
	);
end counter_10bit;

architecture Behavioral of counter_10bit is
	signal big_counter : integer;
	signal LEDT : integer
begin
	process (MAX10_CLK1_50, KEY(0)) 
	begin
		if KEY(0) = '1' then
			LEDT <= 0;
			big_counter <= 0;
	elsif rising_edge(MAX10_CLK1_50) then
			big_counter <= big_counter + 1;
			if big_counter = 100000000 then
				big_counter <= 0;
				LEDT <= LEDT + 1;
				if LEDT > 1023 then
					LEDT = 0;
				end if;
				LEDR <= std_logic_vector(LEDT); 
			end if;
		end if;
	end process;		
end Behavioral;