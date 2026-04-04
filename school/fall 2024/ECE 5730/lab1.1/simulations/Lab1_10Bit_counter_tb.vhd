library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity Lab1_10Bit_counter_tb is
end Lab1_10Bit_counter_tb;

architecture behavioral of Lab1_10Bit_counter_tb is

	component Lab1_10Bit_counter
		port (
			ADC_CLK_10 : in std_logic;
			MAX10_CLK1_50 : in std_logic;
			MAX10_CLK2_50 : in std_logic;
			KEY : in std_logic_vector(1 downto 0);
			LEDR : out std_logic_vector(9 downto 0)
		);
	end component Lab1_10Bit_counter;
	
	signal clk : std_logic := '0';
	signal key : std_logic_vector(1 downto 0) := "00";
	signal LEDR : std_logic_vector(9 downto 0);
	
	constant CLK_PERIOD : time := 10 ns;
	
begin

	uut : Lab1_10Bit_counter
		port map (
			ADC_CLK_10 => clk,
			MAX10_CLK1_50 => clk,
			MAX10_CLK2_50 => clk,
			KEY => key,
			LEDR => LEDR
		);
		
	clk_process : process
	begin
		clk <= '0';
		wait for clk_period / 2;
		clk <= '1';
		wait for clk_period/2;
	end process;
	
	stm_process : process
	begin
		KEY <= "01";
		wait for clk_period * 20;
		KEY <= "00";
		wait for CLK_PERIOD * 5;
		KEY <= "01";
		wait;
	end process;

end architecture behavioral;
	