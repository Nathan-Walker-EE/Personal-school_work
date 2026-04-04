library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity debouncer is
	port (
		clk : in std_logic;
		btn : in std_logic;
		output : out std_logic
	);
end;

architecture states of debouncer is
	type state_type is (WAITING, PRESSED, HOLD, RELEASED);
	signal current_state, next_state: state_type;
begin
	
	process(clk) begin
		if rising_edge(clk) then
			current_state <= next_state;
		end if;
	end process;
	
	process (current_state, btn)
	begin
		case current_state is
			when WAITING =>
				output <= '0';
				if btn = '1' then
					next_state <= PRESSED;
				else
					next_state <= WAITING;
				end if;
			
			when PRESSED =>
				output <= '0';
				if btn = '1' then
					next_state <= HOLD;
				else
					next_state <= WAITING;
				end if;
				
			when HOLD =>
				output <= '0';
				if btn = '1' then
					next_state <= HOLD;
				else
					next_state <= RELEASED;
				end if;
			
			when RELEASED =>
				if btn = '1' then
					next_state <= HOLD;
				else
					next_state <= WAITING;
					output <= '1';
				end if;
			
			when others =>
				output <= '0';
				next_state <= WAITING;
		end case;
	end process;
end states;