using System.Windows;
using System.Windows.Input;
using WPFWeather.ViewModel;

namespace WPFWeather.Windows
{
    public partial class MainWindow : Window
    {
        WeatherViewModel viewmodel = new WeatherViewModel();
        int i;
        public MainWindow()
        {
            InitializeComponent();

            // Set DataContext of this window to main ViewModel of application
            
            DataContext = viewmodel;
            viewmodel.DownloadWeather("Praha");

            string[] CityNames = new string[3] { "Praha", "London", "Brno" };
            cbx.ItemsSource = CityNames;
           
        }

        private void cbx_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            string str = cbx.SelectedItem.ToString();
            viewmodel.DownloadWeather(str);
        }
    }
}
