library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity HEX_seven_seg_disp_tb is
end HEX_seven_seg_disp_tb;

architecture behavioral of HEX_seven_seg_disp_tb is

	component HEX_seven_seg_disp
		port (
			hex : in std_logic_vector(3 downto 0);
			clk : in std_logic;
			oseg : out std_logic_vector(6 downto 0)
		);
	end component HEX_seven_seg_disp;
	
	signal hex : std_logic_vector(3 downto 0) := "0000";
	signal oseg : std_logic_vector(6 downto 0);
	signal my_int : integer;
	signal clk : std_logic;
	constant CLK_PERIOD : time := 10 ns;
	
begin

	uut : HEX_seven_seg_disp
		port map (
			hex => hex,
			clk => clk,
			oseg => oseg
		);
		
	clk_process : process
	begin 
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
	end process;
	
	stm_process : process
	begin
		my_int <= to_integer(unsigned(hex)); -- Convert to integer
		my_int <= my_int + 1;                  -- Increment the integer
		if my_int > 15 then
			my_int <= 0;
		end if;
		hex <= std_logic_vector(to_unsigned(my_int, 4));
		wait for clk_period * 20;
	end process;
	
end behavioral;