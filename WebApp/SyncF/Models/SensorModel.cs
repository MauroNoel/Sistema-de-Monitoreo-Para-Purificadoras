using Google.Protobuf;

namespace SyncF.Models
{
    public class SensorModel
    {
        public long id { get; set; }
        public DateTime fecha { get; set; }
        public long hora_mexico { get; set; }
        public float valor { get; set; }
        public string sensor { get; set; }
    }
}
