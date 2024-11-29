using System.Collections.Generic;
using System.Collections.Concurrent;
using System;
using UnityEngine;

namespace Flakkari4Unity
{
    public class Synchronizer : MonoBehaviour
    {
        private readonly Dictionary<ulong, ECS.Entity> entities = new();
        private static readonly ConcurrentQueue<Action> actions = new();

        public static void Enqueue(Action action)
        {
            actions.Enqueue(action);
        }

        private void Update()
        {
            while (actions.TryDequeue(out var action))
            {
                action.Invoke();
            }
        }

        public void AddEntity(ulong id, ECS.Entity entity, byte[] payload)
        {
            if (entities.ContainsKey(id))
            {
                Debug.LogWarning($"Entity with id {id} already exists.");
                return;
            }

            entity.Id = id;

            entity.CreateComponents(payload);

            entities.Add(id, entity);
        }

        public void RemoveEntity(ulong id)
        {
            if (entities.TryGetValue(id, out ECS.Entity entity))
            {
                entities.Remove(id);
                Destroy(entity.gameObject);
            }
        }

        public ECS.Entity GetEntity(ulong id)
        {
            return entities[id];
        }
    }
}
