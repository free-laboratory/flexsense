clear, clc 
load('cleaned_data.mat')

% start from (target) matrix (n by 3)
seg1_s1 = rb1_interp - rb0_interp;
seg2_s2 = rb2_interp - rb1_interp;
seg3_s3 = rb3_interp - rb2_interp;
seg4_s4 = rb4_interp - rb3_interp;
seg5_s5 = rb5_interp - rb4_interp;
seg6_s6 = rb6_interp - rb5_interp;
seg7_s7 = rb7_interp - rb6_interp;

% data (input) (n by 1 vector) => seg1_s1 
sensor_data25 = sensor_data(:,25);
sensor_data26 = sensor_data(:,26);

% data (input) (n by 1 vector) => seg2_s2
sensor_data21 = sensor_data(:,21);
sensor_data22 = sensor_data(:,22);
sensor_data23 = sensor_data(:,23);
sensor_data24 = sensor_data(:,24);

% data (input) (n by 1 vector) => seg3_s3
sensor_data17 = sensor_data(:,17);
sensor_data18 = sensor_data(:,18);
sensor_data19 = sensor_data(:,19);
sensor_data20 = sensor_data(:,20);

% data (input) (n by 1 vector) => seg4_s4
sensor_data13 = sensor_data(:,13);
sensor_data14 = sensor_data(:,14);
sensor_data15 = sensor_data(:,15);
sensor_data16 = sensor_data(:,16);

% data (input) (n by 1 vector) => seg5_s5
sensor_data9 = sensor_data(:,9);
sensor_data10 = sensor_data(:,10);
sensor_data11 = sensor_data(:,11);
sensor_data12 = sensor_data(:,12);

% data (input) (n by 1 vector) => seg6_s6
sensor_data5 = sensor_data(:,5);
sensor_data6 = sensor_data(:,6);
sensor_data7 = sensor_data(:,7);
sensor_data8 = sensor_data(:,8);

% data (input) (n by 1 vector) => seg7_s7
sensor_data1 = sensor_data(:,1);
sensor_data2 = sensor_data(:,2);
sensor_data3 = sensor_data(:,3);
sensor_data4 = sensor_data(:,4);

%% Learning
% --- Define input-output data pairs for each segment ---
inputs = {...
    [sensor_data25, sensor_data26], seg1_s1; ...
    [sensor_data21, sensor_data22, sensor_data23, sensor_data24], seg2_s2; ...
    [sensor_data17, sensor_data18, sensor_data19, sensor_data20], seg3_s3; ...
    [sensor_data13, sensor_data14, sensor_data15, sensor_data16], seg4_s4; ...
    [sensor_data9, sensor_data10, sensor_data11, sensor_data12], seg5_s5; ...
    [sensor_data5, sensor_data6, sensor_data7, sensor_data8], seg6_s6; ...
    [sensor_data1, sensor_data2, sensor_data3, sensor_data4], seg7_s7};

models = cell(size(inputs, 1), 1);  % Store multivariate regression models

% --- Train multivariate regression model for each segment ---
for i = 1:size(inputs, 1)
    X = inputs{i, 1};  % (n by m) input sensor data
    Y = inputs{i, 2};  % (n by 3) target segment displacement

    % Add bias term (ones column) to X for intercept
    X = [ones(size(X,1), 1), X];

    % Train multivariate regression model using mvregress()
    B = mvregress(X, Y);

    % Store the learned coefficients
    models{i} = B;

    % Display learned regression coefficients
    fprintf('Segment %d Regression Coefficients:\n', i);
    disp(B);
end

%% Evaluation - 1
% --- Evaluate and plot model performance ---
figure;
hold on;
grid on;
colors = lines(7); % Different colors for each segment

for i = 1:size(inputs, 1)
    X = inputs{i, 1};  % Input sensor data (n by m)
    Y_actual = inputs{i, 2};  % Ground truth (n by 3)
    
    % Add bias term to X
    X = [ones(size(X,1), 1), X];

    % Compute predicted values
    Y_pred = X * models{i};  % (n by 3) prediction

    % Plot actual vs predicted for X-axis
    subplot(3, 1, 1); 
    hold on;
    plot(Y_actual(:,1), 'Color', colors(i,:), 'LineWidth', 1.5, 'DisplayName', sprintf('Segment %d Actual', i));
    plot(Y_pred(:,1), '--', 'Color', colors(i,:), 'LineWidth', 1.5, 'DisplayName', sprintf('Segment %d Predicted', i));
    title('X-axis: Actual vs. Predicted');
    ylabel('Displacement (X)');

    % Plot actual vs predicted for Y-axis
    subplot(3, 1, 2); 
    hold on;
    plot(Y_actual(:,2), 'Color', colors(i,:), 'LineWidth', 1.5);
    plot(Y_pred(:,2), '--', 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Y-axis: Actual vs. Predicted');
    ylabel('Displacement (Y)');

    % Plot actual vs predicted for Z-axis
    subplot(3, 1, 3);
    hold on;
    plot(Y_actual(:,3), 'Color', colors(i,:), 'LineWidth', 1.5);
    plot(Y_pred(:,3), '--', 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Z-axis: Actual vs. Predicted');
    ylabel('Displacement (Z)');
end

subplot(3, 1, 1);
legend;
subplot(3, 1, 3);
xlabel('Time Frames');

hold off;

%% Evaluation 2 (Error)
% --- Error Plot (Residuals) ---
figure;
hold on;
grid on;

for i = 1:size(inputs, 1)
    X = inputs{i, 1};  % Input sensor data (n by m)
    Y_actual = inputs{i, 2};  % Ground truth (n by 3)
    
    % Add bias term to X
    X = [ones(size(X,1), 1), X];

    % Compute predicted values
    Y_pred = X * models{i};  % (n by 3) prediction

    % Compute residual errors
    errors = Y_actual - Y_pred;  % (n by 3) residuals

    % Plot error for each axis
    subplot(3, 1, 1);
    hold on;
    plot(errors(:,1), 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Residual Error: X-axis');
    ylabel('Error (X)');

    subplot(3, 1, 2);
    hold on;
    plot(errors(:,2), 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Residual Error: Y-axis');
    ylabel('Error (Y)');

    subplot(3, 1, 3);
    hold on;
    plot(errors(:,3), 'Color', colors(i,:), 'LineWidth', 1.5);
    title('Residual Error: Z-axis');
    ylabel('Error (Z)');
end

subplot(3, 1, 3);
xlabel('Time Frames');

hold off;