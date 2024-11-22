using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components.Common
{
    public class Child : MonoBehaviour
    {
        public string _name;

        internal void Deserialize(byte[] data, ref int i)
        {
            ulong length = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            _name = System.Text.Encoding.UTF8.GetString(data, i, (int)length);
            i += (int)length;
        }
    }
}
