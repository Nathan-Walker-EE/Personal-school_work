library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

--insert name of file i.e. debouncer_tb
entity reg_bank_tb is
end reg_bank_tb;

architecture behavioral of reg_bank_tb is
	
	component reg_bank 
		generic (
			DATA_WIDTH	:	integer	:= 32;
			ADDR_WIDTH	:	integer	:= 5
		);
		port (
		  clk            : in  std_logic;
        rst            : in  std_logic;
        reg_read_addr1 : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
        reg_read_addr2 : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
        reg_write_addr : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
        reg_write_data : in  std_logic_vector(DATA_WIDTH-1 downto 0);
        reg_write_en   : in  std_logic;
        reg_read_data1 : out std_logic_vector(DATA_WIDTH-1 downto 0);
        reg_read_data2 : out std_logic_vector(DATA_WIDTH-1 downto 0)
		);
	end component reg_bank;
	
	DATA_WIDTH	:	integer	:= 32;
	ADDR_WIDTH	:	integer	:= 5;
	signal clk 			: 	std_logic;
	signal rst			:	std_logic;
	signal tb_reg_read_addr1 : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
   signal tb_reg_read_addr2 : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
   signal tb_reg_write_addr : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
   signal tb_reg_write_data : in  std_logic_vector(DATA_WIDTH-1 downto 0);
   signal tb_reg_write_en   : in  std_logic;
   signal tb_reg_read_data1 : out std_logic_vector(DATA_WIDTH-1 downto 0);
   signal tb_reg_read_data2 : out std_logic_vector(DATA_WIDTH-1 downto 0)

	--necessary to progress simulation
	constant CLK_PERIOD : time := 10 ns;
	
begin
	
	-- use ports and signals declared to map to module
	uut : reg_bank
		generic map (
			DATA_WIDTH=> DATA_WIDTH,
			ADDR_WIDTH=> ADDR_WIDTH
		)
		port map(
			clk=>clk,
			rst=>tb_rst,
			addr_out1=>tb_addr_out1,
			addr_out2=>tb_addr_out2,
			wr1=>tb_wr1,
			data_in=>tb_data_in,
			addr_in=>tb_addr_in
			data_out1=>tb_data_out1,
			data_out2=>tb_data_out2
		);
		
	--process to set the clock
	--keep this the same for all simulations
	clk_process : process
	begin
		clk <= '0';
		wait for clk_period / 2;
		clk <= '1';
		wait for clk_period/2;
	end process;

	--manipulate inputs to module to view results you want
	stm_process : process
	begin
		tb_addr_1<= "00000";
		tb_addr_2<= "00000";
		tb_wr1	<= '0';
		tb_data_in <= "10101";
		tb_addr_in <= "00000";
		wait for CLK_PERIOD;
		
		tb_addr_1<= "00000";
		tb_addr_2<= "00000";
		tb_wr1	<= '1';
		tb_data_in <= "10101";
		tb_addr_in <= "00000";
		wait for CLK_PERIOD;
		
		tb_addr_1<= "00000";
		tb_addr_2<= "00000";
		tb_wr1	<= '0';
		tb_data_in <= "10101";
		tb_addr_in <= "00000";
		wait for CLK_PERIOD;
		
		tb_addr_1<= "10110";
		tb_addr_2<= "00000";
		tb_wr1	<= '1';
		tb_data_in <= "10101";
		tb_addr_in <= "00000";
		wait for CLK_PERIOD;
		
		tb_addr_1<= "10110";
		tb_addr_2<= "00000";
		tb_wr1	<= '1';
		tb_data_in <= "11011";
		tb_addr_in <= "00000";
		wait for CLK_PERIOD;
		
		tb_addr_1<= "00000";
		tb_addr_2<= "10110";
		tb_wr1	<= '0';
		tb_data_in <= "00000";
		tb_addr_in <= "00000";
		wait for CLK_PERIOD;
		
	end process;
	
end architecture behavioral;