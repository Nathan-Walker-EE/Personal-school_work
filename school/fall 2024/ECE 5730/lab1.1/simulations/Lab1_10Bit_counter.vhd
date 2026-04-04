library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;  -- To handle conversions between integers and std_logic_vector


entity Lab1_10Bit_counter is
   port (
       ADC_CLK_10 : in std_logic;
       MAX10_CLK1_50 : in std_logic;  -- 50 MHz clock
       MAX10_CLK2_50 : in std_logic;
       KEY : in std_logic_vector(1 downto 0);  -- Reset button
       LEDR : out std_logic_vector(9 downto 0)  -- 10-bit LED output
   );
end Lab1_10Bit_counter;


architecture Behavioral of Lab1_10Bit_counter is
   signal led_num : integer := 0;  -- LED counter signal
   signal counter : integer := 0;  -- Clock cycle counter signal
   constant goal : integer := 2;  -- Count up to goal (to slow down the LED update)
								 --25000000
begin
   process (MAX10_CLK1_50, KEY(0))
   begin
       if KEY(0) = '0' then  -- Reset when button is pressed
           led_num <= 0;      -- Reset LED counter
           counter <= 0;      -- Reset cycle counter
           LEDR <= (others => '0');  -- Turn off all LEDs
       elsif rising_edge(MAX10_CLK1_50) then  -- Increment on clock edge
           if counter = goal then
               counter <= 0;  -- Reset cycle counter after reaching the goal
               if led_num = 1023 then
                   led_num <= 0;  -- Reset LED counter after reaching 1023
               else
                   led_num <= led_num + 1;  -- Increment LED counter
               end if;
               LEDR <= std_logic_vector(to_unsigned(led_num, 10));  -- Update LEDs
           else
               counter <= counter + 1;  -- Increment cycle counter
           end if;
       end if;
   end process;
end Behavioral;