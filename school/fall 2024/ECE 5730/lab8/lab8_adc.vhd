library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity lab8_adc is
	port 
	(
		-- CLOCK --
		ADC_CLK_10 : in std_logic;
		MAX10_CLK1_50 : in std_logic;
		MAX10_CLK2_50 : in std_logic;
		
		-- seg7 -- 
		HEX0 std_logic_vector(7 downto 0);
		HEX1 std_logic_vector(7 downto 0);
		HEX2 std_logic_vector(7 downto 0);
		HEX3 std_logic_vector(7 downto 0);
		HEX4 std_logic_vector(7 downto 0);
		HEX5 std_logic_vector(7 downto 0);

		-- KEY --
		KEY : in std_logic_vector(1 downto 0);
		
	);
end entity lab8_adc

architecture component_list of lab8_adc is
	component HEX_seven_seg_disp_6
		port (
			IN0 : in std_logic_vector(3 downto 0);
			IN1 : in std_logic_vector(3 downto 0);
			IN2 : in std_logic_vector(3 downto 0);
			IN3 : in std_logic_vector(3 downto 0);
			IN4 : in std_logic_vector(3 downto 0);
			IN5 : in std_logic_vector(3 downto 0);
			clk : in  std_logic;
			HEX0 : out std_logic_vector(7 downto 0);
			HEX1 : out std_logic_vector(7 downto 0);
			HEX2 : out std_logic_vector(7 downto 0);
			HEX3 : out std_logic_vector(7 downto 0);
			HEX4 : out std_logic_vector(7 downto 0);
			HEX5 : out std_logic_vector(7 downto 0)
		);
	end component HEX_seven_seg_disp_6;
	
	component ADC1 is
		port (
			clock_clk              : in  std_logic                     := 'X';             -- clk
			reset_sink_reset_n     : in  std_logic                     := 'X';             -- reset_n
			adc_pll_clock_clk      : in  std_logic                     := 'X';             -- clk
			adc_pll_locked_export  : in  std_logic                     := 'X';             -- export
			command_valid          : in  std_logic                     := 'X';             -- valid
			command_channel        : in  std_logic_vector(4 downto 0)  := (others => 'X'); -- channel
			command_startofpacket  : in  std_logic                     := 'X';             -- startofpacket
			command_endofpacket    : in  std_logic                     := 'X';             -- endofpacket
			command_ready          : out std_logic;                                        -- ready
			response_valid         : out std_logic;                                        -- valid
			response_channel       : out std_logic_vector(4 downto 0);                     -- channel
			response_data          : out std_logic_vector(11 downto 0);                    -- data
			response_startofpacket : out std_logic;                                        -- startofpacket
			response_endofpacket   : out std_logic                                         -- endofpacket
		);
	end component ADC1;
	
	signal clk : std_logic;
	signal locked : std_logic;
	signal cmd_val : std_logic := '1';
	signal cmd_chan : std_logic_vector(4 downto 0);
	signal cmd_sop : std_logic := '1';
	signal cmd_eop : std_logic := '1';
	signal cmd_rd : std_logic;
	signal rps_val : std_logic;
	signal rps_chan : std_logic_vector(4 downto 0);
	signal rps_data : std_logic_vector(11 downto 0);
	signal rps_sop : std_logic;
	signal rps_eop : std_logic;
	
	signal turn_off : std_logic_vector(3 downto 0) := (others => '0')
	
begin

	display : HEX_seven_seg_disp_6
		port map (
			clk => rd_clk,
			IN0 => sum(3 downto 0),
			IN1 => sum(7 downto 4), 
			IN2 => sum(11 downto 8),
			IN3 => turn_off,
			IN4 => turn_off,
			IN5 => turn_off,
			HEX0 => HEX0,
			HEX1 => HEX1,
			HEX2 => HEX2,
			HEX3 => HEX3,
			HEX4 => HEX4,
			HEX5 => HEX5
		);
		
	u0 : component ADC1
		port map (
			clock_clk              => ADC_CLK_10,             --          clock.clk
			reset_sink_reset_n     => '1',     --     reset_sink.reset_n
			adc_pll_clock_clk      => clk,      --  adc_pll_clock.clk
			adc_pll_locked_export  => locked,  -- adc_pll_locked.export
			command_valid          => cmd_val          --        command.valid
			command_channel        => cmd_chan,        --               .channel
			command_startofpacket  => cmd_sop,  --               .startofpacket
			command_endofpacket    => cmd_eop,    --               .endofpacket
			command_ready          => cmd_rd,          --               .ready
			response_valid         => rps_val,         --       response.valid
			response_channel       => rps_chan,       --               .channel
			response_data          => rps_data,          --               .data
			response_startofpacket => rps_sop, --               .startofpacket
			response_endofpacket   => rps_eop--               .endofpacket
		);

end component_list;