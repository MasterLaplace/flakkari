using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components._3D
{
    public class BoxCollider : UnityEngine.BoxCollider
    {
        internal void Deserialize(byte[] data, ref int i)
        {
            center = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
            i += sizeof(float) * 3;
            size = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
            i += sizeof(float) * 3;
        }
    }
}
