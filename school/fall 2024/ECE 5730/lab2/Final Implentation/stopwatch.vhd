library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity stopwatch is
	port (
		HEX0 : out std_logic_vector(7 downto 0);
		HEX1 : out std_logic_vector(7 downto 0);
		HEX2 : out std_logic_vector(7 downto 0);
		HEX3 : out std_logic_vector(7 downto 0);
		HEX4 : out std_logic_vector(7 downto 0);
		HEX5 : out std_logic_vector(7 downto 0)
		ADC_CLK_10 : IN STD_LOGIC;
		MAX10_CLK1_10 : IN STD_LOGIC;
		MAX10_CLK2_10 : IN STD_LOGIC;
		KEY : IN STD_LOGIC_VECTOR(1 DOWNTO 0)
	);
end stopwatch;


architecture componentlist of stopwatch is
	component HEX_seven_seg_disp_6
		port (
			clk : in std_logic;
			IN0 : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
			IN1 : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
			IN2 : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
			IN3 : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
			IN4 : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
			IN5 : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
			HEX0 : in std_logic_vector(6 downto 0);
			HEX1 : in std_logic_vector(6 downto 0);
			HEX2 : in std_logic_vector(6 downto 0);
			HEX3 : in std_logic_vector(6 downto 0);
			HEX4 : in std_logic_vector(6 downto 0);
			HEX5 : in std_logic_vector(6 downto 0)
		);
	end component HEX_seven_seg_disp_6;
	
	component Lab2_stopwatch
		port (
			ADC_CLK_10 : in STD_LOGIC;
			MAX10_CLK1_50 : in STD_LOGIC;
			MAX10_CLK2_50 : in STD_LOGIC;
			KEY : in std_logic_vector(1 downto 0);
			HEX0 : out STD_LOGIC_VECTOR(7 downto 0);
			HEX1 : out STD_LOGIC_VECTOR(7 downto 0);
			HEX2 : out STD_LOGIC_VECTOR(7 downto 0);
			HEX3 : out STD_LOGIC_VECTOR(7 downto 0);
			HEX4 : out STD_LOGIC_VECTOR(7 downto 0);
			HEX5 : out STD_LOGIC_VECTOR(7 downto 0)
		);
	end component Lab2_stopwatch;
	
begin

end componentlist;