library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RNG is
	generic (
		N : integer := 8; --output number
		M : integer := 8; --number in LFSR
	);
	
	port (
		clk : in std_logic;
		en : in std_logic;
		rst : in std_logic;
		rn : out std_logic_vector( N-1 down to 0)
	);
end RNG;

architecture behavioral of RNG is
	component LFSR
		generic (
			M : integer := 8;
		);
		port (
			clk : in std_logic; 
			en : in std_logic;
			rst : in std_logic;
			input : in std_logic;
			data : out std_logic_vector(M-1 downto 0)
		);
	end component LFSR;
	
	signal input : std_logic;
	
begin
	LFSR1 : LFSR
		generic map (
			M => 8
		);
		port map (
			clk => clk,
			en => en,
			rst => rst,
			input => input,
			data => rn
		);
		
	process clk begin
		-- fill out logic for input
	end process
end behavioral;
	