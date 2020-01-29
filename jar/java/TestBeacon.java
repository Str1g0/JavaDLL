package jar.lib;

import java.lang.*;
import javax.swing.JOptionPane;

class TestBeacon {

	private int m_value;
	
	public TestBeacon(int val)
	{
		m_value = val;
	}
	
	public void PrintStuff()
	{
		JOptionPane.showMessageDialog(null, "Hello from java in c++. Your value is " + m_value);
	}
	
	public static TestBeacon NewBeacon(int value) {
		return new TestBeacon(value);
	}
}