INSERT INTO engineers (full_name, email, role, department) VALUES
('Ava Santos', 'ava.santos@example.com', 'Junior Engineer', 'IT Support'),
('Miguel Reyes', 'miguel.reyes@example.com', 'Senior Engineer', 'Infrastructure'),
('Nina Cruz', 'nina.cruz@example.com', 'Support Engineer', 'Client Success');

INSERT INTO tickets (title, description, requester_name, requester_email, category, priority, status, assigned_engineer_id, resolution_notes) VALUES
('Cannot access VPN', 'User cannot connect to the company VPN from home.', 'Liam Chen', 'liam.chen@example.com', 'Network Issue', 'High', 'Open', 2, NULL),
('Laptop battery swelling', 'Requester reports visible swelling near the laptop trackpad.', 'Mia Garcia', 'mia.garcia@example.com', 'Hardware Issue', 'Critical', 'In Progress', 2, NULL),
('Password reset loop', 'Password reset email sends but the new password does not work.', 'Noah Smith', 'noah.smith@example.com', 'Account Access', 'Medium', 'Resolved', 1, 'Reset identity provider session and confirmed login.'),
('CRM export failing', 'CSV export fails with a generic server error.', 'Emma Davis', 'emma.davis@example.com', 'System Bug', 'High', 'Open', 3, NULL),
('Install design software', 'Marketing team needs design software installed on a new workstation.', 'Olivia Brown', 'olivia.brown@example.com', 'Client Request', 'Low', 'Closed', 1, 'Software installed and license assigned.'),
('Email sync delayed', 'Mailbox updates are delayed by more than one hour.', 'James Wilson', 'james.wilson@example.com', 'Software Issue', 'Medium', 'In Progress', 1, NULL),
('Office printer offline', 'Shared printer on the third floor is not reachable.', 'Sophia Miller', 'sophia.miller@example.com', 'Hardware Issue', 'Medium', 'Open', NULL, NULL),
('Production bug report', 'Client reports intermittent 500 errors on checkout.', 'Benjamin Lee', 'benjamin.lee@example.com', 'System Bug', 'Critical', 'In Progress', 3, NULL),
('New employee account', 'Create accounts and tool access for a new hire.', 'Isabella Clark', 'isabella.clark@example.com', 'Account Access', 'Low', 'Resolved', 1, 'Provisioned email, VPN, and project tracker accounts.'),
('Slow Wi-Fi in meeting room', 'Video calls drop frequently in meeting room B.', 'Lucas Hall', 'lucas.hall@example.com', 'Network Issue', 'High', 'Open', 2, NULL);

INSERT INTO activity_logs (ticket_id, action, description) VALUES
(1, 'TICKET_CREATED', 'Ticket created: Cannot access VPN'),
(2, 'TICKET_ASSIGNED', 'Ticket assigned to Miguel Reyes: Laptop battery swelling'),
(3, 'TICKET_RESOLVED', 'Ticket resolved after identity provider session reset.'),
(5, 'TICKET_CLOSED', 'Ticket closed after software installation.'),
(8, 'TICKET_UPDATED', 'Ticket updated with production checkout details.');
