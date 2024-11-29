using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components.Common
{
    public class Evolve : MonoBehaviour
    {
        public string _name;

        internal void Deserialize(byte[] data, ref int i)
        {
            uint length = BitConverter.ToUInt32(data, i);
            i += sizeof(uint);
            _name = System.Text.Encoding.UTF8.GetString(data, i, (int)length);
            i += (int)length;
        }
    }
}
