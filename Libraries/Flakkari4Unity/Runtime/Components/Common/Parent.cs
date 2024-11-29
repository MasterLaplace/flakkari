using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components.Common
{
    public class Parent : MonoBehaviour
    {
        public ulong entity;

        internal void Deserialize(byte[] data, ref int i)
        {
            entity = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
        }
    }
}
