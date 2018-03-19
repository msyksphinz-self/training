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

public final class sw extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "sw",
          sw.class,
          null
          );
      addArgument("rd", REG.INFO);
      addArgument("offset", SHORT);
      addArgument("address", REG.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rd = (REG) getArgument("rd", args);
      final Immediate offset = (Immediate) getArgument("offset", args);
      final REG address = (REG) getArgument("address", args);

      return new sw(rd, offset, address);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "sw",
          instruction.class,
          null
          );
      addArgument("rd", REG.INFO);
      addArgument("offset", SHORT);
      addArgument("address", REG.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rd = (REG) getArgument("rd", args);
      final Immediate offset = (Immediate) getArgument("offset", args);
      final REG address = (REG) getArgument("address", args);

      return new instruction(
        new sw(
          rd,
          offset,
          address
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rd;
  public final Immediate offset;
  public final REG address;

  public sw(final REG rd, final Immediate offset, final REG address) {
    assert REG.INFO.isSupported(rd);
    assert SHORT.equals(offset.getType());
    assert REG.INFO.isSupported(address);

    this.rd = rd;
    this.offset = offset;
    this.address = address;

    addArgument("rd", rd);
    addArgument("offset", offset);
    addArgument("address", address);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("sw %s, %d(%s)", rd.text(vars__), offset.access().load().bigIntegerValue(), address.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("101011%s%s%s", address.image(vars__), rd.image(vars__), offset.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    pe__.M.access(address.access(pe__, vars__).load().add(Data.signExtend(WORD, offset.access().load())).shiftRight(Data.valueOf(WORD, 0x2))).store(rd.access(pe__, vars__).load());
  }
}