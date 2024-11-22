using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components.Common
{
    public class Timer : MonoBehaviour
    {
        public ulong lastTime;
        public float maxTime;

        internal void Deserialize(byte[] data, ref int i)
        {
            lastTime = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            maxTime = BitConverter.ToSingle(data, i);
            i += sizeof(float);
        }
    }
}
