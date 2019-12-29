package com.surfyn.portal.task;

import com.surfyn.portal.repository.TaskRepository;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;

@Service
class TaskService {

    private final TaskRepository taskRepository;

    public TaskService(TaskRepository taskRepository) {
        this.taskRepository = taskRepository;
    }

    Iterable<Task> findAll() {
        return taskRepository.findAll();
    }

    public Task findOne(Long id) {
        return taskRepository.getOne(id);
    }
}
