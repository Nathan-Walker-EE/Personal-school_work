library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ADC is
	port (
		clk : in std_logic;
		btn : in std_logic;
		output1 : out std_logic_vector(11 downto 0);
		output2 : out std_logic_vector(11 downto 0)
	);
end;
		
architecture counter of ADC is
	component base_ADC is
		port (
			clock_clk              	: in  std_logic                     := 'X';             -- clk
			reset_sink_reset_n     	: in  std_logic                     := 'X';             -- reset_n
			adc_pll_clock_clk      	: in  std_logic                     := 'X';             -- clk
			adc_pll_locked_export  	: in  std_logic                     := 'X';             -- export
			command_valid          	: in  std_logic                     := 'X';             -- valid
			command_channel        	: in  std_logic_vector(4 downto 0)  := (others => 'X'); -- channel
			command_startofpacket  	: in  std_logic                     := 'X';             -- startofpacket
			command_endofpacket    	: in  std_logic                     := 'X';             -- endofpacket
			command_ready          	: out std_logic;                                        -- ready
			response_valid         	: out std_logic;                                        -- valid
			response_channel       	: out std_logic_vector(4 downto 0);                     -- channel
			response_data          	: out std_logic_vector(11 downto 0);                    -- data
			response_startofpacket 	: out std_logic;                                        -- startofpacket
			response_endofpacket   	: out std_logic                                         -- endofpacket
		);
	end component base_ADC;

	component PLL_10M is
		port (
			inclk0					: IN STD_LOGIC  := '0';
			c0						: OUT STD_LOGIC ;
			locked					: OUT STD_LOGIC 
		);
	end component PLL_10M;
	
	type state_type is (read_1, wait1, read_2, wait2);
	signal current_state, next_state: state_type;

	signal s_clock_clk              : std_logic;
	-- signal s_reset_sink_reset_n     : std_logic;
	signal s_adc_pll_clock_clk      : std_logic;
	signal s_adc_pll_locked_export  : std_logic;
	signal s_command_valid          : std_logic := '1';
	signal s_command_channel        : std_logic_vector(4 downto 0) := "00001";
	signal s_command_startofpacket  : std_logic := '1';
	signal s_command_endofpacket    : std_logic := '1';
	signal s_command_ready          : std_logic;                                        -- ready
	signal s_response_valid         : std_logic;                                        -- valid
	signal s_response_channel       : std_logic_vector(4 downto 0);
	signal s_response_data          : std_logic_vector(11 downto 0);
	signal s_response_startofpacket : std_logic;                                        -- startofpacket
	signal s_response_endofpacket   : std_logic;                                         -- endofpacket

	-- signal temp_Data				: std_logic_vector(11 downto 0);


	-- signal count : integer := 0;
	-- signal v_counter : integer := 0;
begin

	u0 : component base_ADC
		port map (
			clock_clk              => clk,              --          clock.clk
			reset_sink_reset_n     => btn,     --     reset_sink.reset_n
			adc_pll_clock_clk      => s_adc_pll_clock_clk,      --  adc_pll_clock.clk
			adc_pll_locked_export  => s_adc_pll_locked_export,  -- adc_pll_locked.export
			command_valid          => s_command_valid,          --        command.valid
			command_channel        => s_command_channel,        --               .channel
			command_startofpacket  => s_command_startofpacket,  --               .startofpacket
			command_endofpacket    => s_command_endofpacket,    --               .endofpacket
			command_ready          => s_command_ready,          --               .ready
			response_valid         => s_response_valid,         --       response.valid
			response_channel       => s_response_channel,       --               .channel
			response_data          => s_response_data,          --               .data
			response_startofpacket => s_response_startofpacket, --               .startofpacket
			response_endofpacket   => s_response_endofpacket    --               .endofpacket
		);

	PLL_10M_inst : PLL_10M PORT MAP (
		inclk0	 => clk,
		c0	 => s_adc_pll_clock_clk,
		locked	 => s_adc_pll_locked_export
	);
	
	clk_proc : process(clk)
	begin
		if rising_edge(clk) then
			current_state <= next_state;
		end if;
	end process clk_proc;
	
	state_proc : process(clk)
	begin
		if rising_edge(clk) then
			case current_state is
				when read_1 =>
					s_command_valid <= '1';
					s_command_channel <= "00001";
					next_state <= wait1;
				when wait1 =>
					s_command_valid <= '1';
					if s_command_ready = '1' then
						next_state <= read_2;
					end if;
				when read_2 =>
					s_command_valid <= '1';
					s_command_channel <= "00010";
					next_state <= wait2;
				when wait2 =>
					s_command_valid <= '1';
					if s_command_ready = '1' then
						next_state <= read_1;
					end if;
			end case;
		end if;
			
	end process state_proc;

	proc1: process(clk)
	begin
		if rising_edge(clk) then
			if btn = '0' then
				output1 <= (others => '0');
				output2 <= (others => '0');
			elsif s_response_valid = '1' and s_command_channel = "00010" then
				output2 <= s_response_data;
			elsif s_response_valid = '1' and s_command_channel = "00001" then
				output1 <= s_response_data;
			end if;
		end if;
	end process proc1;
end counter;