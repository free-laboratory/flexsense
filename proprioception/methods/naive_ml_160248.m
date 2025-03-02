clear, clc 
load('cleaned_data.mat')

% Compute segment displacements (n by 3)
seg1_s1 = rb1_interp - rb0_interp;
seg2_s2 = rb2_interp - rb1_interp;
seg3_s3 = rb3_interp - rb2_interp;
seg4_s4 = rb4_interp - rb3_interp;
seg5_s5 = rb5_interp - rb4_interp;
seg6_s6 = rb6_interp - rb5_interp;
seg7_s7 = rb7_interp - rb6_interp;

% Define input sensor data for each segment
inputs = {...
    [sensor_data(:,25), sensor_data(:,26)], seg1_s1; ...
    [sensor_data(:,21), sensor_data(:,22), sensor_data(:,23), sensor_data(:,24)], seg2_s2; ...
    [sensor_data(:,17), sensor_data(:,18), sensor_data(:,19), sensor_data(:,20)], seg3_s3; ...
    [sensor_data(:,13), sensor_data(:,14), sensor_data(:,15), sensor_data(:,16)], seg4_s4; ...
    [sensor_data(:,9), sensor_data(:,10), sensor_data(:,11), sensor_data(:,12)], seg5_s5; ...
    [sensor_data(:,5), sensor_data(:,6), sensor_data(:,7), sensor_data(:,8)], seg6_s6; ...
    [sensor_data(:,1), sensor_data(:,2), sensor_data(:,3), sensor_data(:,4)], seg7_s7};

models = cell(size(inputs, 1), 1);  % Store multivariate regression models
errors_all = cell(size(inputs, 1), 1);  % Store residual errors for all segments

% Train multivariate regression models
for i = 1:size(inputs, 1)
    X = inputs{i, 1};  % Input sensor data (n by m)
    Y = inputs{i, 2};  % Target segment displacement (n by 3)

    % Add bias term (ones column) to X
    X = [ones(size(X,1), 1), X];

    % Train multivariate regression model using mvregress()
    B = mvregress(X, Y);

    % Store the learned coefficients
    models{i} = B;

    % Compute predicted displacement
    Y_pred = X * B;

    % Compute and store residual errors
    errors_all{i} = Y - Y_pred;

    % Display regression coefficients
    fprintf('Segment %d Regression Coefficients:\n', i);
    disp(B);
end

%% Plot residual errors for all segments
figure;
hold on;
grid on;
colors = lines(7); % Different colors for each segment

for i = 1:size(inputs, 1)
    errors = errors_all{i}; % Retrieve residual errors for segment i

    % Plot error for X-axis
    subplot(3, 1, 1);
    hold on;
    plot(errors(:,1), 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Residual Error: X-axis');
    ylabel('Error (X)');

    % Plot error for Y-axis
    subplot(3, 1, 2);
    hold on;
    plot(errors(:,2), 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Residual Error: Y-axis');
    ylabel('Error (Y)');

    % Plot error for Z-axis
    subplot(3, 1, 3);
    hold on;
    plot(errors(:,3), 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Residual Error: Z-axis');
    ylabel('Error (Z)');
end

subplot(3, 1, 3);
xlabel('Time Frames');
hold off;

%% Compute and plot absolute error for all segments
figure;
hold on;

for i = 1:size(inputs, 1)
    errors = errors_all{i}; % Retrieve residual errors for segment i
    errorSeg = abs(sqrt(errors(:,1).^2 + errors(:,2).^2 + errors(:,3).^2)); % Compute absolute error

    % Plot absolute error for each segment
    plot(errorSeg, 'Color', colors(i,:), 'LineWidth', 1.5, 'DisplayName', sprintf('Segment %d', i));
end

title('Absolute Residual Error for Each Segment');
xlabel('Time Frames');
ylabel('Absolute Error');
legend;
hold off;
