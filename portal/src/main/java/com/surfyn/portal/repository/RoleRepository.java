package com.surfyn.portal.repository;


import com.surfyn.portal.entity.AppRole;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;


@Repository
public interface RoleRepository extends JpaRepository<AppRole, Long> {

    List<AppRole> findAll();

}
