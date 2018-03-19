/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.minimips.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.minimips.PE;
import ru.ispras.microtesk.model.minimips.TempVars;
import ru.ispras.microtesk.model.minimips.mode.*;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class addiu extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "addiu",
          addiu.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("rs", REG.INFO);
      addArgument("imm", SHORT);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final REG rs = (REG) getArgument("rs", args);
      final Immediate imm = (Immediate) getArgument("imm", args);

      return new addiu(rt, rs, imm);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "addiu",
          instruction.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("rs", REG.INFO);
      addArgument("imm", SHORT);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final REG rs = (REG) getArgument("rs", args);
      final Immediate imm = (Immediate) getArgument("imm", args);

      return new instruction(
        new addiu(
          rt,
          rs,
          imm
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rt;
  public final REG rs;
  public final Immediate imm;

  public addiu(final REG rt, final REG rs, final Immediate imm) {
    assert REG.INFO.isSupported(rt);
    assert REG.INFO.isSupported(rs);
    assert SHORT.equals(imm.getType());

    this.rt = rt;
    this.rs = rs;
    this.imm = imm;

    addArgument("rt", rt);
    addArgument("rs", rs);
    addArgument("imm", imm);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("addiu %s, %s, 0x%x", rt.text(vars__), rs.text(vars__), imm.access().load().bigIntegerValue(false));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("001001%s%s%s", rs.image(vars__), rt.image(vars__), imm.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    rt.access(pe__, vars__).store(rs.access(pe__, vars__).load().add(Data.signExtend(Type.INT(32), imm.access().load())));
  }
}