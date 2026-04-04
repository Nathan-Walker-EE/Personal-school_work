architecture component_list of RX_UART is

    type bits is (start, data, stop);
    type oversample_t is (throw_away_start, acquire, throw_away_end);

    signal bit_state     : bits := start;
    signal sample_state  : oversample_t := throw_away_start;

    signal oversample_counter : integer := 0;
    signal one_count          : integer := 0;
    signal zero_count         : integer := 0;
    signal data_count         : integer := 0;

    signal true_data  : std_logic := '0';
    signal sample_rdy : std_logic := '0';

begin

    process (Pclk)
    begin
        if rising_edge(Pclk) then

            -- =========================
            -- Bit-level FSM
            -- =========================
            case bit_state is
                when start =>
                    if sample_rdy = '1' and true_data = '0' then
                        bit_state <= data;
                        data_count <= 0;
                    end if;

                when data =>
                    if sample_rdy = '1' then
                        data_in(data_count) <= true_data;
                        data_count <= data_count + 1;

                        if data_count = 7 then
                            bit_state <= stop;
                        end if;
                    end if;

                when stop =>
                    if sample_rdy = '1' and true_data = '1' then
                        wrreq <= '1';
                        bit_state <= start;
                    end if;
            end case;

            -- =========================
            -- Oversample FSM
            -- =========================
            case sample_state is
                when throw_away_start =>
                    sample_rdy <= '0';
                    if oversample_counter < 1 then
                        oversample_counter <= oversample_counter + 1;
                    else
                        oversample_counter <= 0;
                        one_count  <= 0;
                        zero_count <= 0;
                        sample_state <= acquire;
                    end if;

                when acquire =>
                    if oversample_counter < 3 then
                        oversample_counter <= oversample_counter + 1;
                        if Rx = '0' then
                            zero_count <= zero_count + 1;
                        else
                            one_count <= one_count + 1;
                        end if;
                    else
                        oversample_counter <= 0;
                        sample_rdy <= '1';

                        if zero_count > one_count then
                            true_data <= '0';
                        else
                            true_data <= '1';
                        end if;

                        sample_state <= throw_away_end;
                    end if;

                when throw_away_end =>
                    sample_rdy <= '0';
                    if oversample_counter < 1 then
                        oversample_counter <= oversample_counter + 1;
                    else
                        oversample_counter <= 0;
                        sample_state <= throw_away_start;
                    end if;
            end case;

        end if;
    end process;

end component_list;
