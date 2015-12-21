import java.io.IOException;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier;
//import gnu.io.NoSuchPortException;
//import gnu.io.PortInUseException;
import gnu.io.SerialPort;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JButton;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JSlider;
import java.awt.Color;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;
import java.awt.event.InputMethodListener;
import java.awt.event.InputMethodEvent;
import javax.swing.border.LineBorder;


public class serial_window extends JFrame {
	
	CommPortIdentifier port;
	SerialPort serial;
	OutputStream out;
	char led_select = 0;
	char leds_rgb[][] = new char[4][3];
	boolean connection_est = false;
	
	private JPanel contentPane;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					serial_window frame = new serial_window();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public serial_window() {
		setTitle("2x2 RGBluetooth");
		final JSlider slider_red = new JSlider();
		final JSlider slider_green = new JSlider();
		final JSlider slider_blue = new JSlider();
		final JPanel panel = new JPanel();
		final JPanel panel_1 = new JPanel();
		final JPanel panel_2 = new JPanel();
		panel_2.setBorder(new LineBorder(new Color(0, 0, 0)));
		final JPanel panel_3 = new JPanel();
		panel_3.setBorder(new LineBorder(new Color(0, 0, 0)));
		panel.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent arg0) {
				panel.setBorder(new LineBorder(new Color(0,0,0), 3));
				panel_1.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_2.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_3.setBorder(new LineBorder(new Color(0,0,0), 1));
				led_select = 0;
				slider_red.setValue(leds_rgb[led_select][0]);
				slider_green.setValue(leds_rgb[led_select][1]);
				slider_blue.setValue(leds_rgb[led_select][2]);
			}
		});
		panel_2.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				panel.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_1.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_2.setBorder(new LineBorder(new Color(0,0,0), 3));
				panel_3.setBorder(new LineBorder(new Color(0,0,0), 1));
				led_select = 2;
				slider_red.setValue(leds_rgb[led_select][0]);
				slider_green.setValue(leds_rgb[led_select][1]);
				slider_blue.setValue(leds_rgb[led_select][2]);
			}
		});
		panel_3.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				panel.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_1.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_2.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_3.setBorder(new LineBorder(new Color(0,0,0), 3));
				led_select = 3;
				slider_red.setValue(leds_rgb[led_select][0]);
				slider_green.setValue(leds_rgb[led_select][1]);
				slider_blue.setValue(leds_rgb[led_select][2]);
			}
		});
		panel.setBorder(new LineBorder(new Color(0, 0, 0)));
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 363, 202);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		
		JButton btnConnect = new JButton("Connect");
		btnConnect.setBounds(15, 15, 115, 23);
		btnConnect.addMouseListener(new MouseAdapter() {

			public void mouseClicked(MouseEvent arg0) {
				try {
					port = CommPortIdentifier.getPortIdentifier("COM40");
					serial = (SerialPort) port.open("ales", 300);
					out = serial.getOutputStream();
					connection_est = true;
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				System.out.println("Hello world!");
			}
		});
		
		JButton btnSend = new JButton("Send");
		btnSend.setBounds(140, 15, 75, 23);
		btnSend.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				if (connection_est == true)
				{
				}
			}
		});
		contentPane.setLayout(null);
		contentPane.add(btnSend);
		contentPane.add(btnConnect);
		
		JButton btnCloseConnection = new JButton("Close Connection");
		btnCloseConnection.setBounds(223, 15, 117, 23);
		btnCloseConnection.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent arg0) {
				try {
					out.flush();
					connection_est = false;
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				serial.close();
			}
		});
		btnCloseConnection.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
			}
		});
		contentPane.add(btnCloseConnection);
		
		slider_blue.setBackground(new Color(0, 0, 255));
		slider_blue.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				leds_rgb[led_select][2] = (char) slider_blue.getValue();
				Color color = new Color((int) (leds_rgb[led_select][0] * 2.55), (int) (leds_rgb[led_select][1] * 2.55), (int) (leds_rgb[led_select][2] * 2.55));
				switch (led_select)
				{
					case 0:
						panel.setBackground(color);
						break;
					case 1:
						panel_1.setBackground(color);
						break;
					case 2:
						panel_2.setBackground(color);
						break;
					case 3:
						panel_3.setBackground(color);
						break;
					default:
						break;
				}
				if (connection_est == true)
				{
					try {
						out.write('e');
						out.write(leds_rgb[0][0]);
						out.write(leds_rgb[0][1]);
						out.write(leds_rgb[0][2]);
						
						out.write('g');
						out.write(leds_rgb[1][0]);
						out.write(leds_rgb[1][1]);
						out.write(leds_rgb[1][2]);;
						
						out.write('f');
						out.write(leds_rgb[2][0]);
						out.write(leds_rgb[2][1]);
						out.write(leds_rgb[2][2]);
						
						out.write('h');
						out.write(leds_rgb[3][0]);
						out.write(leds_rgb[3][1]);
						out.write(leds_rgb[3][2]);
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
			}
		});
		slider_blue.setBounds(140, 128, 200, 32);
		slider_blue.setMinorTickSpacing(1);
		slider_blue.setPaintLabels(true);
		slider_blue.setPaintTicks(true);
		slider_blue.setSnapToTicks(true);
		slider_blue.setValue(0);
		contentPane.add(slider_blue);
		
		slider_green.setBackground(new Color(50, 205, 50));
		slider_green.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent arg0) {
				leds_rgb[led_select][1] = (char) slider_green.getValue();
				Color color = new Color((int) (leds_rgb[led_select][0] * 2.55), (int) (leds_rgb[led_select][1] * 2.55), (int) (leds_rgb[led_select][2] * 2.55));
				switch (led_select)
				{
					case 0:
						panel.setBackground(color);
						break;
					case 1:
						panel_1.setBackground(color);
						break;
					case 2:
						panel_2.setBackground(color);
						break;
					case 3:
						panel_3.setBackground(color);
						break;
					default:
						break;
				}
				if (connection_est == true)
				{
					try {
						out.write('e');
						out.write(leds_rgb[0][0]);
						out.write(leds_rgb[0][1]);
						out.write(leds_rgb[0][2]);
						
						out.write('g');
						out.write(leds_rgb[1][0]);
						out.write(leds_rgb[1][1]);
						out.write(leds_rgb[1][2]);;
						
						out.write('f');
						out.write(leds_rgb[2][0]);
						out.write(leds_rgb[2][1]);
						out.write(leds_rgb[2][2]);
						
						out.write('h');
						out.write(leds_rgb[3][0]);
						out.write(leds_rgb[3][1]);
						out.write(leds_rgb[3][2]);
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
			}
		});
		slider_green.setBounds(140, 89, 200, 32);
		slider_green.setMinorTickSpacing(1);
		slider_green.setPaintLabels(true);
		slider_green.setPaintTicks(true);
		slider_green.setSnapToTicks(true);
		slider_green.setValue(0);
		contentPane.add(slider_green);
		
		slider_red.setBackground(Color.RED);
		slider_red.addInputMethodListener(new InputMethodListener() {
			public void caretPositionChanged(InputMethodEvent arg0) {

			}
			public void inputMethodTextChanged(InputMethodEvent arg0) {
			}
		});
		slider_red.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent arg0) {
				leds_rgb[led_select][0] = (char) (slider_red.getValue());
				Color color = new Color((int) (leds_rgb[led_select][0] * 2.55), (int) (leds_rgb[led_select][1] * 2.55), (int) (leds_rgb[led_select][2] * 2.55));
				switch (led_select)
				{
					case 0:
						panel.setBackground(color);
						break;
					case 1:
						panel_1.setBackground(color);
						break;
					case 2:
						panel_2.setBackground(color);
						break;
					case 3:
						panel_3.setBackground(color);
						break;
					default:
						break;
				}
	
				if (connection_est == true)
				{
					try {
						out.write('e');
						out.write(leds_rgb[0][0]);
						out.write(leds_rgb[0][1]);
						out.write(leds_rgb[0][2]);
						
						out.write('g');
						out.write(leds_rgb[1][0]);
						out.write(leds_rgb[1][1]);
						out.write(leds_rgb[1][2]);;
						
						out.write('f');
						out.write(leds_rgb[2][0]);
						out.write(leds_rgb[2][1]);
						out.write(leds_rgb[2][2]);
						
						out.write('h');
						out.write(leds_rgb[3][0]);
						out.write(leds_rgb[3][1]);
						out.write(leds_rgb[3][2]);
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
			}
		});
		slider_red.setBounds(140, 49, 200, 32);
		slider_red.setMinorTickSpacing(1);
		slider_red.setPaintLabels(true);
		slider_red.setPaintTicks(true);
		slider_red.setSnapToTicks(true);
		slider_red.setValue(0);
		contentPane.add(slider_red);
		
		
		panel.setBackground(Color.BLACK);
		panel.setForeground(new Color(255, 0, 0));
		panel.setBounds(15, 49, 50, 50);
		contentPane.add(panel);
		
		panel_1.setBorder(new LineBorder(new Color(0, 0, 0)));
		panel_1.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent arg0) {
				panel.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_1.setBorder(new LineBorder(new Color(0,0,0), 3));
				panel_2.setBorder(new LineBorder(new Color(0,0,0), 1));
				panel_3.setBorder(new LineBorder(new Color(0,0,0), 1));
				led_select = 1;
				slider_red.setValue(leds_rgb[led_select][0]);
				slider_green.setValue(leds_rgb[led_select][1]);
				slider_blue.setValue(leds_rgb[led_select][2]);
			}
		});
		panel_1.setForeground(Color.RED);
		panel_1.setBackground(Color.BLACK);
		panel_1.setBounds(80, 49, 50, 50);
		contentPane.add(panel_1);
		
		panel_2.setForeground(Color.RED);
		panel_2.setBackground(Color.BLACK);
		panel_2.setBounds(15, 110, 50, 50);
		contentPane.add(panel_2);
		
		panel_3.setForeground(Color.RED);
		panel_3.setBackground(Color.BLACK);
		panel_3.setBounds(80, 110, 50, 50);
		contentPane.add(panel_3);
	}
}
