library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity HEX_seven_seg_disp_6 is
	port (
		SW : in std_logic_vector(9 downto 0);
		ADC_CLK_10 : in std_logic;
		MAX10_CLK1_50 : in std_logic;
		MAX10_CLK2_50 : in std_logic;
		HEX0 : out std_logic_vector(7 downto 0);
		HEX1 : out std_logic_vector(7 downto 0);
		HEX2 : out std_logic_vector(7 downto 0);
		HEX3 : out std_logic_vector(7 downto 0);
		HEX4 : out std_logic_vector(7 downto 0);
		HEX5 : out std_logic_vector(7 downto 0)
	);
end HEX_seven_seg_disp_6;

architecture component_list of HEX_seven_seg_disp_6 is
	component HEX_seven_seg_disp
		port (
			clk : in std_logic;
			hex : in std_logic_vector(3 downto 0);
			oseg : out std_logic_vector(6 downto 0)
		);
	end component HEX_seven_seg_disp;
begin

	disp1 : HEX_seven_seg_disp
		port map (
			clk => ADC_CLK_10,
			hex => SW(3 downto 0),
			oseg => HEX0(6 downto 0)
		);
	disp2 : HEX_seven_seg_disp
		port map (
			clk => ADC_CLK_10,
			hex => SW(3 downto 0),
			oseg => HEX1(6 downto 0)
		);
	disp3 : HEX_seven_seg_disp
		port map (
			clk => ADC_CLK_10,
			hex => SW(3 downto 0),
			oseg => HEX2(6 downto 0)
		);
	disp4 : HEX_seven_seg_disp
		port map (
			clk => ADC_CLK_10,
			hex => SW(3 downto 0),
			oseg => HEX3(6 downto 0)
		);
	disp5 : HEX_seven_seg_disp
		port map (
			clk => ADC_CLK_10,
			hex => SW(3 downto 0),
			oseg => HEX4(6 downto 0)
		);
	disp6 : HEX_seven_seg_disp
		port map (
			clk => ADC_CLK_10,
			hex => SW(3 downto 0),
			oseg => HEX5(6 downto 0)
		);
		

end component_list;