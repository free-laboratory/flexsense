clear, clc, close all 

%% Data Cleaning
% Load sensor data
sensor = load('../data_aq/data_logs/com_data_20250208_160248.mat');
gt = readmatrix('../data_gt/160248.xlsx');

% Extract time and sensor data
time_sensor = sensor.data(:, 1)/1000; % in s
sensor_data = sensor.data(:, 2:end); 
sensor_data = sensor_data(:,1:26); % delete NaN columns 

% Extract time and ground truth 
timeDiff = 3.423;
time_gt = gt(6:end, 2) - timeDiff;

rb7 = gt(6:end, 3:5); rb6 = gt(6:end, 15:17); rb5 = gt(6:end, 27:29);
rb4 = gt(6:end, 39:41); rb3 = gt(6:end, 51:53); rb2 = gt(6:end, 63:65);
rb1 = gt(6:end, 78:80); rb0 = gt(6:end, 90:92);

% "Swap" the second and third columns for correct (x, y, z) order
rb0 = rb0(:, [1, 3, 2]); rb1 = rb1(:, [1, 3, 2]); rb2 = rb2(:, [1, 3, 2]);
rb3 = rb3(:, [1, 3, 2]); rb4 = rb4(:, [1, 3, 2]); rb5 = rb5(:, [1, 3, 2]);
rb6 = rb6(:, [1, 3, 2]); rb7 = rb7(:, [1, 3, 2]);

% resampling - resolution of time_gt is better (DOWN SAMPLING)
rb7_interp = interp1(time_gt, rb7, time_sensor, 'linear', 'extrap');
rb6_interp = interp1(time_gt, rb6, time_sensor, 'linear', 'extrap');
rb5_interp = interp1(time_gt, rb5, time_sensor, 'linear', 'extrap');
rb4_interp = interp1(time_gt, rb4, time_sensor, 'linear', 'extrap');
rb3_interp = interp1(time_gt, rb3, time_sensor, 'linear', 'extrap');
rb2_interp = interp1(time_gt, rb2, time_sensor, 'linear', 'extrap');
rb1_interp = interp1(time_gt, rb1, time_sensor, 'linear', 'extrap');
rb0_interp = interp1(time_gt, rb0, time_sensor, 'linear', 'extrap');
time = time_sensor;

% Save Cleaned Data
clear colors gt i legend_entries sensor ans
save('cleaned_data_160248.mat', 'time', 'rb0_interp', 'rb1_interp', 'rb2_interp', ...
    'rb3_interp', 'rb4_interp', 'rb5_interp', 'rb6_interp', 'rb7_interp',...
    'sensor_data')

%% PLOT
% Plot all sensor data
figure(1)
plot(time_sensor, sensor_data)
xlabel('Time [s]')
ylabel('Sensor Readings')
title('Sensor Data Over Time')
legend(arrayfun(@(x) sprintf('Sensor %d', x), 1:size(sensor_data, 2), 'UniformOutput', false))

% Create a 3D figure and set up the plot
figure(2)
hold on;
xlabel('X-axis'); ylabel('Y-axis'); zlabel('Z-axis');
title('3D Position of Points Over Time');
view(3); % Set 3D view

% Define different colors for each trajectory
colors = lines(8); % Use MATLAB's default color scheme for distinction

% Store all rb data in a cell array for loop iteration
rb_data = {rb0, rb1, rb2, rb3, rb4, rb5, rb6, rb7};

% Initialize legend entries
legend_entries = cell(1, length(rb_data));

% Iterate through each dataset and plot its trajectory in 3D space
for i = 1:length(rb_data)
    rb = rb_data{i}; % Extract current dataset
    plot3(rb(:,1), rb(:,2), rb(:,3), 'Color', colors(i,:), 'LineWidth', 1.5);
    
    % Store legend entry for current dataset
    legend_entries{i} = sprintf('rb%d', i-1);
end

% Add a legend to distinguish the different datasets
legend(legend_entries, 'Location', 'best');
axis("equal")
hold off;

% Sampling plot
figure(3)
hold on
title('interp1 check');
plot(time, -sensor_data(:,1)-250)
plot(time, rb7_interp(:,1))
legend('sensor', 'gt')