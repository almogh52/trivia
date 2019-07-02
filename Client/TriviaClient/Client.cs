using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    static class Client
    {
        private const int SERVER_PORT = 61110;
        private const int HEADER_LEN = 5;
        private static TcpClient socket { get; set; }
        private static NetworkStream stream { get; set; }

        static Client()
        {
            // Add destructor
            AppDomain.CurrentDomain.ProcessExit += Destructor;
        }

        private static void Destructor(object sender, EventArgs e)
        {
            // Close the socket
            socket.Close();
        }

        /// <summary>
        /// Connects to the server
        /// </summary>
        /// <returns>
        /// Task
        /// </returns>
        /// <param name="serverIP">The ip of the server to connect to</param>
        public async static Task Connect(string serverIP)
        {
            // Create the tcp client to the server
            socket = new TcpClient();
            await socket.ConnectAsync(serverIP, SERVER_PORT);

            // Get the stream of the socket
            stream = socket.GetStream();
        }

        /// <summary>
        /// Sends a message to the server
        /// </summary>
        /// <returns>
        /// Task
        /// </returns>
        /// <param name="messageId">The id of the message</param>
        /// <param name="message">The bytes of the message</param>
        public static async Task Send(byte messageId, byte[] message)
        {
            byte[] buf = new byte[message.Length + HEADER_LEN];

            // Set the message id
            buf[0] = messageId;

            // Copy the message length
            Array.Copy(BitConverter.GetBytes(message.Length), 0, buf, 1, sizeof(int));

            // Copy the message
            Array.Copy(message, 0, buf, HEADER_LEN, message.Length);

            // Write to the stream the buffer
            await stream.WriteAsync(buf, 0, buf.Length);
        }

        /// <summary>
        /// Recv a message from the server
        /// </summary>
        /// <returns>
        /// A byte array containing the response
        /// </returns>
        public static async Task<byte[]> Recv()
        {
            byte[] messageSizeBuf = new byte[4];
            int messageSize;

            byte[] buf;

            // Read the message size
            await stream.ReadAsync(messageSizeBuf, 0, 4);
            messageSize = BitConverter.ToInt32(messageSizeBuf, 0);

            // Read the message
            buf = new byte[messageSize];
            await stream.ReadAsync(buf, 0, messageSize);

            return buf;
        }
    }
}