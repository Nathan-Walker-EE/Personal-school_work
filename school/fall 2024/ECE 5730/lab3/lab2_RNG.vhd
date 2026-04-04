library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lab2_RNG is
	generic (
		N : integer := 8
	);
	port (
		HEX0 : out std_logic_vector(N - 1 downto 0);
		HEX1 : out std_logic_vector(N - 1 downto 0);
		HEX2 : out std_logic_vector(N - 1 downto 0);
		HEX3 : out std_logic_vector(N - 1 downto 0);
		HEX4 : out std_logic_vector(N - 1 downto 0);
		HEX5 : out std_logic_vector(N - 1 downto 0);
		KEY0 : in std_logic; --turn to vector not key0 and key1
		KEY1 : in std_logic;
		ADC_clk_10 : in std_logic;
		MAX10_CLK1_50 : in std_logic;
		MAX10_CLK2_50 : in std_logic
	);
end lab2_RNG;

architecture componentlist of lab2_RNG is
	component RNG
		generic (
			N : integer := 8;
			M : integer := 8
		);
		port(
			clk : in std_logic;
			en : in std_logic;
			rst : in std_logic;
			rn : out std_logic_vector ( N - 1 downto 0)
		);
	end component RNG;
	
	component HEX_seven_seg_disp
		port (
			hex : in std_logic_vector(3 downto 0);
			clk : in std_logic;
			oseg : out std_logic_vector(7 downto 0)
		);
	end component HEX_seven_seg_disp;
	
	signal rn : std_logic_vector (N - 1 downto 0);
	

begin

	RNG1 : RNG 
		generic map (
			N => 8,
			M => 8
		);
		port map (
			clk => ADC_CLK_10,
			en => KEY0,
			rst => KEY1,
			rn => rn
		);
		
	disp1 : HEX_seven_seg_disp
		port map (
			hex => rn(3 downto 0),
			clk => ADC_CLK_10,
			oseg => HEX0
		);
	
	disp2 : HEX_seven_seg_disp
		port map (
			hex => rn(7 downto 4),
			clk => ADC_CLK_10,
			oseg => HEX1
		);
	
	HEX2 <= '11111111';
	HEX3 <= '11111111';
	HEX4 <= '11111111';
	HEX5 <= '11111111';
end componentlist;